#include "PGMEMMGR.H"
#include "PGERRORS.H"
#include "PGEXCEPS.H"
#include "PGIO.H"

#ifdef POSIX_PLATFORM

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <new>
#include <unordered_set>
#include <vector>

namespace {

struct ModernHandle {
	size_t size;
	unsigned char data[1];
};

struct ModernMemoryBlock {
	mem_rec rec;
	std::vector<unsigned char> bytes;
	bool disposed;
};

struct ModernMemoryState {
	std::vector<ModernMemoryBlock *> blocks;
};

std::unordered_set<ModernMemoryBlock *> g_blocks;

memory_ref ref_from_block(ModernMemoryBlock *block)
{
	return reinterpret_cast<memory_ref>(block);
}

ModernMemoryBlock *block_from_ref(memory_ref ref)
{
	auto *block = reinterpret_cast<ModernMemoryBlock *>(ref);
	if (!block || block->disposed || g_blocks.find(block) == g_blocks.end())
		return nullptr;
	return block;
}

ModernMemoryState *state_from_globals(pgm_globals_ptr globals)
{
	return globals ? static_cast<ModernMemoryState *>(globals->machine_var) : nullptr;
}

size_t byte_size_for(pg_short_t rec_size, size_t num_recs)
{
	const size_t width = rec_size ? static_cast<size_t>(rec_size) : 1;

	if (num_recs > std::numeric_limits<size_t>::max() / width)
		return std::numeric_limits<size_t>::max();

	return width * num_recs;
}

void fail_if_needed(pgm_globals_ptr globals, void *allocation)
{
	if (!allocation && globals)
		pgFailure(globals, NO_MEMORY_ERR, 0);
}

void update_total_after_resize(ModernMemoryBlock *block, size_t old_bytes, size_t new_bytes)
{
	if (!block || !block->rec.globals)
		return;

	if (new_bytes >= old_bytes)
		block->rec.globals->total_unpurged += new_bytes - old_bytes;
	else
		block->rec.globals->total_unpurged -= old_bytes - new_bytes;
}

void remove_from_state(ModernMemoryBlock *block)
{
	if (!block || !block->rec.globals)
		return;

	if (auto *state = state_from_globals(block->rec.globals)) {
		auto found = std::find(state->blocks.begin(), state->blocks.end(), block);
		if (found != state->blocks.end())
			state->blocks.erase(found);
	}
}

ModernHandle *make_handle(const void *data, size_t size)
{
	auto *handle = static_cast<ModernHandle *>(std::malloc(sizeof(ModernHandle) + size));
	if (!handle)
		return nullptr;

	handle->size = size;
	if (size && data)
		std::memcpy(handle->data, data, size);

	return handle;
}

} // namespace

extern "C" {

PG_PASCAL (void) pgMemStartup(pgm_globals_ptr mem_globals, long max_memory)
{
	std::memset(mem_globals, 0, sizeof(pgm_globals));

	mem_globals->signature = CHECKSUM_SIG;
	mem_globals->debug_proc = pgDebugProc;
	mem_globals->debug_flags = DEF_DEBUG_BITS;
	mem_globals->next_mem_id = mem_globals->current_id = MINIMUM_ID;
	mem_globals->purge_threshold = PURGE_THRESHOLD;
	mem_globals->max_memory = max_memory ? static_cast<size_t>(max_memory) : std::numeric_limits<size_t>::max();

	try {
		mem_globals->machine_var = new ModernMemoryState;
		mem_globals->spare_tire = make_handle(nullptr, 1024);
		mem_globals->total_unpurged = 1024;
	} catch (...) {
		pgMemShutdown(mem_globals);
		pgFailure(mem_globals, NO_MEMORY_ERR, 0);
	}
}

PG_PASCAL (void) pgMemShutdown(pgm_globals_ptr mem_globals)
{
	if (!mem_globals)
		return;

	auto *state = state_from_globals(mem_globals);
	if (state) {
		auto blocks = state->blocks;
		for (auto *block : blocks) {
			if (block) {
				g_blocks.erase(block);
				block->disposed = true;
				delete block;
			}
		}

		delete state;
		mem_globals->machine_var = nullptr;
	}

	if (mem_globals->spare_tire) {
		std::free(mem_globals->spare_tire);
		mem_globals->spare_tire = nullptr;
	}

	mem_globals->master_handle = nullptr;
	mem_globals->master_list = nullptr;
	mem_globals->total_unpurged = 0;
}

PG_PASCAL (memory_ref) DisposeMemory(memory_ref ref)
{
	auto *block = block_from_ref(ref);
	if (!block)
		return MEM_NULL;

	remove_from_state(block);
	if (block->rec.globals)
		block->rec.globals->total_unpurged -= block->bytes.size();

	g_blocks.erase(block);
	block->disposed = true;
	delete block;

	return MEM_NULL;
}

PG_PASCAL (memory_ref) DisposeNonNilMemory(memory_ref ref)
{
	if (ref)
		DisposeMemory(ref);
	return MEM_NULL;
}

PG_PASCAL (void) DisposeUsedMemory(memory_ref ref)
{
	auto *block = block_from_ref(ref);
	if (block)
		block->rec.access = 0;
	DisposeMemory(ref);
}

#ifdef PG_DEBUG
PG_PASCAL (memory_ref) pgAllocateNewRef(pgm_globals_ptr mem_globals, pg_short_t rec_size,
		size_t num_recs, short extend_size, short zero_fill, char *file, int line)
#else
PG_PASCAL (memory_ref) pgAllocateNewRef(pgm_globals_ptr mem_globals, pg_short_t rec_size,
		size_t num_recs, short extend_size, short zero_fill)
#endif
{
	try {
		auto *state = state_from_globals(mem_globals);
		if (!state) {
			state = new ModernMemoryState;
			mem_globals->machine_var = state;
		}

		auto *block = new ModernMemoryBlock;
		std::memset(&block->rec, 0, sizeof(block->rec));
		block->disposed = false;
		block->rec.rec_size = rec_size ? rec_size : 1;
		block->rec.extend_size = extend_size;
		block->rec.num_recs = num_recs;
		block->rec.real_num_recs = num_recs;
		block->rec.master_index = state->blocks.size();
		block->rec.purge = NO_PURGING_STATUS;
		block->rec.mem_id = mem_globals ? mem_globals->current_id : MINIMUM_ID;
		block->rec.globals = mem_globals;
#ifdef PG_DEBUG
		block->rec.line = line;
		if (file)
			std::strncpy(block->rec.file, file, DEBUG_INFO_SIZE - 1);
#endif
		block->bytes.resize(byte_size_for(block->rec.rec_size, num_recs));
		if (zero_fill && !block->bytes.empty())
			std::memset(block->bytes.data(), 0, block->bytes.size());

		state->blocks.push_back(block);
		g_blocks.insert(block);

		if (mem_globals)
			mem_globals->total_unpurged += block->bytes.size();

		return ref_from_block(block);
	} catch (...) {
		if (mem_globals)
			pgFailure(mem_globals, NO_MEMORY_ERR, 0);
		return MEM_NULL;
	}
}

PG_PASCAL (memory_ref) MemoryAllocID(pgm_globals_ptr globals,
		pg_short_t rec_size, size_t num_recs, short extend_size, long mem_id)
{
	memory_ref ref = pgAllocateNewRef(globals, rec_size, num_recs, extend_size, FALSE);
	if (auto *block = block_from_ref(ref))
		block->rec.mem_id = mem_id;
	return ref;
}

PG_PASCAL (memory_ref) MemoryAllocClearID(pgm_globals_ptr globals,
		pg_short_t rec_size, size_t num_recs, short extend_size, long mem_id)
{
	memory_ref ref = pgAllocateNewRef(globals, rec_size, num_recs, extend_size, TRUE);
	if (auto *block = block_from_ref(ref))
		block->rec.mem_id = mem_id;
	return ref;
}

PG_PASCAL (memory_ref) MemoryDuplicate(memory_ref src_ref)
{
	auto *src = block_from_ref(src_ref);
	if (!src)
		return MEM_NULL;

	return MemoryDuplicateID(src_ref, src->rec.mem_id);
}

PG_PASCAL (memory_ref) MemoryDuplicateID(memory_ref src_ref, long mem_id)
{
	auto *src = block_from_ref(src_ref);
	if (!src)
		return MEM_NULL;

	memory_ref copy = MemoryAllocID(src->rec.globals, src->rec.rec_size, src->rec.num_recs,
			src->rec.extend_size, mem_id);
	if (auto *dest = block_from_ref(copy)) {
		dest->bytes = src->bytes;
		dest->rec.real_num_recs = src->rec.real_num_recs;
		dest->rec.purge = src->rec.purge;
	}

	return copy;
}

PG_PASCAL (void) MemoryCopy(memory_ref src_ref, memory_ref target_ref)
{
	auto *src = block_from_ref(src_ref);
	auto *target = block_from_ref(target_ref);
	if (!src || !target)
		return;

	target->rec.rec_size = src->rec.rec_size;
	target->rec.extend_size = src->rec.extend_size;
	SetMemorySize(target_ref, src->rec.num_recs);
	target = block_from_ref(target_ref);
	target->bytes = src->bytes;
}

PG_PASCAL (memory_ref) MemoryRecover(void PG_FAR *ptr)
{
	if (!ptr)
		return MEM_NULL;

	for (auto *block : g_blocks) {
		if (!block || block->bytes.empty())
			continue;

		unsigned char *begin = block->bytes.data();
		unsigned char *end = begin + block->bytes.size();
		auto *candidate = static_cast<unsigned char *>(ptr);
		if (candidate >= begin && candidate <= end)
			return ref_from_block(block);
	}

	return MEM_NULL;
}

PG_PASCAL (void PG_FAR *) UseMemory(memory_ref ref)
{
	auto *block = block_from_ref(ref);
	if (!block)
		return nullptr;

	++block->rec.access;
	++block->rec.qty_used;
	return block->bytes.empty() ? nullptr : block->bytes.data();
}

PG_PASCAL (void PG_FAR *) UseMemoryRecord(memory_ref ref, size_t wanted_rec,
		size_t seq_recs_used, pg_boolean first_use)
{
	(void)seq_recs_used;
	(void)first_use;

	auto *block = block_from_ref(ref);
	if (!block)
		return nullptr;

	if (wanted_rec == static_cast<size_t>(USE_ALL_RECS))
		wanted_rec = 0;

	size_t offset = byte_size_for(block->rec.rec_size, wanted_rec);
	if (offset > block->bytes.size())
		return nullptr;

	++block->rec.access;
	++block->rec.qty_used;
	return block->bytes.data() + offset;
}

PG_PASCAL (void) GetMemoryRecord(memory_ref ref, long wanted_rec, void PG_FAR *record)
{
	auto *block = block_from_ref(ref);
	if (!block || !record || wanted_rec < 0)
		return;

	size_t offset = byte_size_for(block->rec.rec_size, static_cast<size_t>(wanted_rec));
	size_t width = static_cast<size_t>(block->rec.rec_size);
	if (offset + width <= block->bytes.size())
		std::memcpy(record, block->bytes.data() + offset, width);
}

PG_PASCAL (void PG_FAR *) UseForLongTime(memory_ref ref)
{
	return UseMemory(ref);
}

PG_PASCAL (void) SetMemorySize(memory_ref ref_to_size, size_t wanted_size)
{
	auto *block = block_from_ref(ref_to_size);
	if (!block)
		return;

	const size_t old_bytes = block->bytes.size();
	const size_t new_bytes = byte_size_for(block->rec.rec_size, wanted_size);
	try {
		block->bytes.resize(new_bytes);
	} catch (...) {
		pgFailure(block->rec.globals, NO_MEMORY_ERR, 0);
	}

	block->rec.num_recs = wanted_size;
	block->rec.real_num_recs = wanted_size;
	update_total_after_resize(block, old_bytes, new_bytes);
}

PG_PASCAL (void) SetMemoryRecSize(memory_ref ref, pg_short_t new_rec_size, short extend_size)
{
	auto *block = block_from_ref(ref);
	if (!block)
		return;

	const size_t old_bytes = block->bytes.size();
	block->rec.rec_size = new_rec_size ? new_rec_size : 1;
	block->rec.extend_size = extend_size;
	const size_t new_bytes = byte_size_for(block->rec.rec_size, block->rec.num_recs);
	block->bytes.resize(new_bytes);
	update_total_after_resize(block, old_bytes, new_bytes);
}

PG_PASCAL (void) SetMemoryExtendSize(memory_ref ref, short extend_size)
{
	if (auto *block = block_from_ref(ref))
		block->rec.extend_size = extend_size;
}

PG_PASCAL (short) GetMemoryExtendSize(memory_ref ref)
{
	auto *block = block_from_ref(ref);
	return block ? block->rec.extend_size : 0;
}

PG_PASCAL (void) DisposeAllMemory(pgm_globals_ptr mem_globals, long memory_id)
{
	auto *state = state_from_globals(mem_globals);
	if (!state)
		return;

	auto blocks = state->blocks;
	for (auto *block : blocks)
		if (block && (!memory_id || block->rec.mem_id == memory_id))
			DisposeMemory(ref_from_block(block));
}

PG_PASCAL (void) PurgeAllMemory(pgm_globals_ptr mem_globals, long memory_id)
{
	(void)mem_globals;
	(void)memory_id;
}

PG_PASCAL (void) UnuseAllMemory(pgm_globals_ptr mem_globals, long memory_id)
{
	auto *state = state_from_globals(mem_globals);
	if (!state)
		return;

	for (auto *block : state->blocks)
		if (block && (!memory_id || block->rec.mem_id == memory_id))
			block->rec.access = 0;
}

PG_PASCAL (size_t) GetAllMemorySize(pgm_globals_ptr mem_globals, long memory_id,
		pg_boolean return_byte_size)
{
	auto *state = state_from_globals(mem_globals);
	if (!state)
		return 0;

	size_t total = 0;
	for (auto *block : state->blocks) {
		if (block && (!memory_id || block->rec.mem_id == memory_id))
			total += return_byte_size ? block->bytes.size() : block->rec.num_recs;
	}

	return total;
}

PG_PASCAL (void PG_FAR *) InsertMemory(memory_ref ref, size_t offset, size_t insert_size)
{
	auto *block = block_from_ref(ref);
	if (!block)
		return nullptr;

	if (offset > block->rec.num_recs)
		offset = block->rec.num_recs;

	const size_t old_bytes = block->bytes.size();
	const size_t byte_offset = byte_size_for(block->rec.rec_size, offset);
	const size_t insert_bytes = byte_size_for(block->rec.rec_size, insert_size);

	try {
		block->bytes.insert(block->bytes.begin() + byte_offset, insert_bytes, 0);
	} catch (...) {
		pgFailure(block->rec.globals, NO_MEMORY_ERR, 0);
	}

	block->rec.num_recs += insert_size;
	block->rec.real_num_recs = block->rec.num_recs;
	++block->rec.access;
	update_total_after_resize(block, old_bytes, block->bytes.size());

	return block->bytes.data() + byte_offset;
}

PG_PASCAL (void PG_FAR *) AppendMemory(memory_ref ref, size_t append_size, pg_boolean zero_fill)
{
	auto *block = block_from_ref(ref);
	if (!block)
		return nullptr;

	const size_t old_recs = block->rec.num_recs;
	void *result = InsertMemory(ref, old_recs, append_size);
	if (!zero_fill && result)
		std::memset(result, 0, byte_size_for(block->rec.rec_size, append_size));

	return result;
}

PG_PASCAL (void) DeleteMemory(memory_ref ref, size_t offset, size_t delete_size)
{
	auto *block = block_from_ref(ref);
	if (!block || offset >= block->rec.num_recs || !delete_size)
		return;

	if (offset + delete_size > block->rec.num_recs)
		delete_size = block->rec.num_recs - offset;

	const size_t old_bytes = block->bytes.size();
	const size_t byte_offset = byte_size_for(block->rec.rec_size, offset);
	const size_t delete_bytes = byte_size_for(block->rec.rec_size, delete_size);
	block->bytes.erase(block->bytes.begin() + byte_offset, block->bytes.begin() + byte_offset + delete_bytes);
	block->rec.num_recs -= delete_size;
	block->rec.real_num_recs = block->rec.num_recs;
	update_total_after_resize(block, old_bytes, block->bytes.size());
}

PG_PASCAL (void) SetMemoryPurge(memory_ref ref, short purge_priority, pg_boolean no_data_save)
{
	if (auto *block = block_from_ref(ref)) {
		if (purge_priority != static_cast<short>(PURGE_NO_CHANGE))
			block->rec.purge = static_cast<pg_short_t>(purge_priority & PURGE_VALUE_MASK);
		if (no_data_save)
			block->rec.purge |= NO_DATA_SAVE_FLAG;
	}
}

PG_PASCAL (memory_ref) InitMemoryRef(pgm_globals_ptr mem_globals, pg_handle base_ref)
{
	auto *handle = static_cast<ModernHandle *>(base_ref);
	if (!handle)
		return MEM_NULL;

	memory_ref ref = pgAllocateNewRef(mem_globals, 1, handle->size, 0, FALSE);
	if (auto *block = block_from_ref(ref))
		std::memcpy(block->bytes.data(), handle->data, handle->size);

	return ref;
}

PG_PASCAL (memory_ref) ForceMinimumMemorySize(memory_ref ref_to_size)
{
	return ref_to_size;
}

PG_PASCAL (pg_error) MemoryPurge(pgm_globals_ptr globals, size_t minimum_amount,
		memory_ref mask_ref)
{
	(void)globals;
	(void)minimum_amount;
	(void)mask_ref;
	return NO_ERROR;
}

PG_PASCAL (void) InitVirtualMemory(pgm_globals_ptr globals, purge_proc purge_function,
		long ref_con)
{
	globals->purge = purge_function ? purge_function : pgStandardPurgeProc;
	globals->purge_ref_con = ref_con;
}

PG_PASCAL (void) UnuseMemory(memory_ref ref)
{
	if (auto *block = block_from_ref(ref))
		if (block->rec.access > 0)
			--block->rec.access;
}

PG_PASCAL (size_t) GetMemorySize(memory_ref ref)
{
	auto *block = block_from_ref(ref);
	return block ? block->rec.num_recs : 0;
}

PG_PASCAL (size_t) GetByteSize(memory_ref ref)
{
	auto *block = block_from_ref(ref);
	return block ? block->bytes.size() : 0;
}

PG_PASCAL (short) GetMemoryRecSize(memory_ref ref)
{
	auto *block = block_from_ref(ref);
	return block ? block->rec.rec_size : 0;
}

PG_PASCAL (pgm_globals_ptr) GetGlobalsFromRef(memory_ref ref)
{
	auto *block = block_from_ref(ref);
	return block ? block->rec.globals : nullptr;
}

PG_PASCAL (void) UnuseAndDispose(memory_ref ref)
{
	if (auto *block = block_from_ref(ref))
		block->rec.access = 0;
	DisposeMemory(ref);
}

PG_PASCAL (long) GetMemoryRefID(memory_ref ref)
{
	auto *block = block_from_ref(ref);
	return block ? block->rec.mem_id : 0;
}

PG_PASCAL (void) SetMemoryRefID(memory_ref ref, long mem_id)
{
	if (auto *block = block_from_ref(ref))
		block->rec.mem_id = mem_id;
}

PG_PASCAL (short) GetAccessCtr(memory_ref ref)
{
	auto *block = block_from_ref(ref);
	return block ? block->rec.access : 0;
}

PG_PASCAL (long) pgUniqueMemoryID(pgm_globals_ptr globals)
{
	++globals->next_mem_id;
	globals->current_id = globals->next_mem_id;
	return globals->current_id;
}

PG_PASCAL (void) ChangeAllMemoryID(pgm_globals_ptr mem_globals, long orig_id, long new_id)
{
	auto *state = state_from_globals(mem_globals);
	if (!state)
		return;

	for (auto *block : state->blocks)
		if (block && block->rec.mem_id == orig_id)
			block->rec.mem_id = new_id;
}

PG_PASCAL (size_t) pgGetByteSize(pg_short_t rec_size, size_t num_recs)
{
	return byte_size_for(rec_size, num_recs);
}

PG_PASCAL (void) DisposeFailedMemory(memory_ref ref)
{
	DisposeMemory(ref);
}

PG_PASCAL (void) UnuseAllFailedMemory(pgm_globals_ptr mem_globals, long memory_id)
{
	UnuseAllMemory(mem_globals, memory_id);
}

PG_PASCAL (void) UnuseFailedMemory(memory_ref ref)
{
	UnuseMemory(ref);
}

PG_PASCAL (void) DisposeAllFailedMemory(pgm_globals_ptr mem_globals, long memory_id)
{
	DisposeAllMemory(mem_globals, memory_id);
}

PG_PASCAL (pg_handle) DetachMemory(memory_ref ref)
{
	auto *block = block_from_ref(ref);
	if (!block)
		return nullptr;

	pg_handle handle = make_handle(block->bytes.data(), block->bytes.size());
	DisposeMemory(ref);
	return handle;
}

PG_PASCAL (void) pgMemInsert(memory_ref ref, size_t offset, size_t byte_size, size_t block_size)
{
	auto *block = block_from_ref(ref);
	if (!block)
		return;

	if (block_size == 0)
		block_size = 1;

	const size_t old_bytes = block->bytes.size();
	if (offset > old_bytes)
		offset = old_bytes;

	try {
		block->bytes.insert(block->bytes.begin() + offset, byte_size * block_size, 0);
	} catch (...) {
		pgFailure(block->rec.globals, NO_MEMORY_ERR, 0);
	}

	block->rec.num_recs = block->bytes.size() / static_cast<size_t>(block->rec.rec_size);
	block->rec.real_num_recs = block->rec.num_recs;
	update_total_after_resize(block, old_bytes, block->bytes.size());
}

PG_PASCAL (void) pgMemDelete(memory_ref ref, size_t offset, size_t byte_size, size_t block_size)
{
	auto *block = block_from_ref(ref);
	if (!block)
		return;

	if (block_size == 0)
		block_size = 1;

	size_t bytes = byte_size * block_size;
	if (offset >= block->bytes.size())
		return;
	if (offset + bytes > block->bytes.size())
		bytes = block->bytes.size() - offset;

	const size_t old_bytes = block->bytes.size();
	block->bytes.erase(block->bytes.begin() + offset, block->bytes.begin() + offset + bytes);
	block->rec.num_recs = block->bytes.size() / static_cast<size_t>(block->rec.rec_size);
	block->rec.real_num_recs = block->rec.num_recs;
	update_total_after_resize(block, old_bytes, block->bytes.size());
}

PG_PASCAL (void) pgBlockMove(const void PG_FAR *src, void PG_FAR *dest, ptrdiff_t block_size)
{
	if (!block_size || !src || !dest)
		return;

	if (block_size > 0) {
		std::memmove(dest, src, static_cast<size_t>(block_size));
		return;
	}

	size_t byte_count = static_cast<size_t>(-block_size);
	auto *src_bytes = static_cast<const unsigned char *>(src);
	auto *dest_bytes = static_cast<unsigned char *>(dest);
	while (byte_count--)
		*(--dest_bytes) = *(--src_bytes);
}

PG_PASCAL (short) pgCheckReference(memory_ref ref)
{
	return block_from_ref(ref) ? FALSE : TRUE;
}

PG_PASCAL (pg_error) pgStandardPurgeProc(memory_ref ref_to_purge, pgm_globals_ptr mem_globals, short verb)
{
	(void)ref_to_purge;
	(void)mem_globals;
	(void)verb;
	return NO_ERROR;
}

PG_PASCAL (long) pgStandardFreeProc(pgm_globals_ptr mem_globals, memory_ref dont_free, long desired_free)
{
	(void)mem_globals;
	(void)dont_free;
	(void)desired_free;
	return 0;
}

PG_PASCAL (pg_short_t) GetMemoryPurge(memory_ref ref)
{
	auto *block = block_from_ref(ref);
	return block ? block->rec.purge : 0;
}

PG_PASCAL (size_t) MemoryToCStr(memory_ref ref)
{
	size_t result = GetMemorySize(ref);
	AppendMemory(ref, 1, TRUE);
	UnuseMemory(ref);
	return result;
}

PG_PASCAL (size_t) MemoryToPStr(memory_ref ref)
{
	size_t result = GetMemorySize(ref);
	if (GetMemoryRecSize(ref) == static_cast<short>(sizeof(pg_char)) && result < 255) {
		auto *length = static_cast<pg_char_ptr>(InsertMemory(ref, 0, 1));
		if (length)
			*length = static_cast<pg_char>(result);
		UnuseMemory(ref);
	}
	return result;
}

PG_PASCAL (size_t) CStrToMemory(memory_ref ref)
{
	size_t result = GetMemorySize(ref);
	if (result)
		DeleteMemory(ref, result - 1, 1);
	return result ? result - 1 : 0;
}

PG_PASCAL (size_t) PStrToMemory(memory_ref ref)
{
	size_t result = GetMemorySize(ref);
	if (result)
		DeleteMemory(ref, 0, 1);
	return result ? result - 1 : 0;
}

PG_PASCAL (char PG_FAR *) UseMemoryToCStr(memory_ref ref)
{
	MemoryToCStr(ref);
	return static_cast<char PG_FAR *>(UseMemory(ref));
}

PG_PASCAL (pg_char_ptr) UseMemoryToPStr(memory_ref ref)
{
	if (MemoryToPStr(ref) > 255)
		pgFailure(GetGlobalsFromRef(ref), PG_PSTRING_TOO_BIG_ERR, 0);
	return static_cast<pg_char_ptr>(UseMemory(ref));
}

PG_PASCAL (void) UnuseMemoryFromCStr(memory_ref ref)
{
	UnuseMemory(ref);
	CStrToMemory(ref);
}

PG_PASCAL (void) UnuseMemoryFromPStr(memory_ref ref)
{
	UnuseMemory(ref);
	PStrToMemory(ref);
}

PG_PASCAL (void) UnuseAndZero(memory_ref ref)
{
	UnuseMemory(ref);
	SetMemorySize(ref, 0);
}

PG_PASCAL (memory_ref) DisposeNonNilFailedMemory(memory_ref ref)
{
	if (ref)
		DisposeFailedMemory(ref);
	return MEM_NULL;
}

PG_PASCAL (memory_ref) DuplicateNonNilMemory(memory_ref ref)
{
	return ref ? MemoryDuplicate(ref) : MEM_NULL;
}

PG_PASCAL (pg_boolean) EqualMemory(memory_ref ref1, memory_ref ref2)
{
	auto *left = block_from_ref(ref1);
	auto *right = block_from_ref(ref2);
	if (!left || !right)
		return static_cast<pg_boolean>(left == right);
	return static_cast<pg_boolean>(left->rec.rec_size == right->rec.rec_size && left->bytes == right->bytes);
}

PG_PASCAL (memory_ref) Bytes2Memory(pgm_globals_ptr mem_globals, const pg_bits8_ptr data, short char_size, long length)
{
	memory_ref ref = pgAllocateNewRef(mem_globals, static_cast<pg_short_t>(char_size ? char_size : 1),
			length > 0 ? static_cast<size_t>(length) : 0, 0, FALSE);
	if (auto *block = block_from_ref(ref))
		if (data && !block->bytes.empty())
			std::memcpy(block->bytes.data(), data, block->bytes.size());
	return ref;
}

#ifdef PG_DEBUG
PG_PASCAL (pg_boolean) ValidMemoryRef(memory_ref ref)
{
	return static_cast<pg_boolean>(block_from_ref(ref) != nullptr);
}

PG_PASCAL (pg_boolean) pgCheckAllMemoryRefs(void)
{
	return FALSE;
}
#endif

PG_PASCAL (memory_ref) HandleToMemory(pgm_globals *mem_globals, pg_handle handle, pg_short_t rec_size)
{
	auto *modern = static_cast<ModernHandle *>(handle);
	if (!modern)
		return MEM_NULL;

	size_t records = rec_size ? (modern->size + static_cast<size_t>(rec_size) - 1) / static_cast<size_t>(rec_size) : modern->size;
	memory_ref ref = pgAllocateNewRef(mem_globals, rec_size ? rec_size : 1, records, 0, FALSE);
	if (auto *block = block_from_ref(ref)) {
		if (!block->bytes.empty())
			std::memset(block->bytes.data(), 0, block->bytes.size());
		if (modern->size)
			std::memcpy(block->bytes.data(), modern->data, modern->size);
	}

	return ref;
}

PG_PASCAL (pg_handle) MemoryToHandle(memory_ref ref)
{
	auto *block = block_from_ref(ref);
	if (!block)
		return nullptr;
	return make_handle(block->bytes.data(), block->bytes.size());
}

} // extern "C"

#endif
