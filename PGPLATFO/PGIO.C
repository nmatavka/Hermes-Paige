/* 
	Paige Universal I/O
	
	File:		pgIO.c

	by T. R. Shaw

	Copyright � 1995 OITC, Inc.
	All rights reserved.
		
	pgIO.c - Universal I/O routines

	This software has been privately developed and falls within 
	DFARS 252.227-7013(c)(1) and associated regulations and it or its
	derivatives may not be used outside of the License agreement with 
	T. R. Shaw and OITC.  Copyright may not be removed.
	
	oitc@iu.net

	Revision History
	03/27/95	1.0b1	TRS - Initial beta for external customer Paige release
	03/18/96	1.4 GC - Moved pgScrapMemoryRead/Write
*/

#include "PGIO.H"
#include "PGOSUTL.H"
#include "DEFPROCS.H"


/* Names indicate functions in this file */


#ifdef MAC_PLATFORM

#include <Aliases.h>
#include <Files.h>

#pragma segment pgbasic4


/* Create a file descriptor */

PG_C (pg_file_desc_ref) pgFileSpec2FileDescriptor(const pgm_globals_ptr mem_globals, const pg_file_desc_ptr spec)
{
	pg_file_desc_ref	ref;

	ref = MemoryAlloc (mem_globals, sizeof(FSSpec), 1, 0);
	*((FSSpecPtr)UseMemory(ref)) = *spec;
	UnuseMemory(ref);
	
	return ref;
}


/* Create a file descriptor */

PG_C (pg_file_desc_ref) pgCreateFileDescriptor(const pgm_globals_ptr mem_globals, const pg_file_name_ptr file_name)
{
	FSSpec				spec;
	
	pgFailError(mem_globals, FSMakeFSSpec(0, 0, file_name, &spec));
	
	return pgFileSpec2FileDescriptor(mem_globals, &spec);
}


#ifdef NO_C_INLINE

/* Dispose of a file descriptor */

PG_C (void) pgDisposeFileDescriptor(pg_file_desc_ref ref)
{
	DisposeNonNilFailedMemory(ref);
}

#endif


/* Create a file and open it */

PG_C (pg_error) pgCreateFile(pg_file_desc_ref ref, short perm, pg_file_unit_ptr ref_num)
{
	long		type = GetGlobalsFromRef(ref)->fileType;
	long		creator = GetGlobalsFromRef(ref)->creator;
	pg_error	err;
	
	if (perm & PG_BINARY)
	{
		if (!type) 
			type = PG_BINARY_TYPE;
	}
	else type = 'TEXT';
	
	if (!creator) 
		creator = PG_IO_CREATOR;

	if ((err = FSpCreate((FSSpecPtr)UseMemory(ref), creator, type, smSystemScript)) == dupFNErr)
	{
		if (perm & PG_EXCL)
			err = NO_ERROR;
		if (!err)
			err = pgOpenFile(ref, perm, ref_num);
	}
	else
	if (err == NO_ERROR)
		err = pgOpenFile(ref, perm, ref_num);

	return err;
}


/* Open a file */

PG_C (pg_error) pgOpenFile(pg_file_desc_ref ref, short perm, pg_file_unit_ptr ref_num)
{
	FInfo		fndrInfo;
	FSSpecPtr	spec;
	pg_error	err;
	Boolean		wasAliased;
	Boolean		targetIsFolder;
		
	spec = UseMemory(ref);
	
	err = ResolveAliasFile(spec, TRUE, &targetIsFolder, &wasAliased);
	if (targetIsFolder)
		err = fnfErr;
	if (!err)
	{
		if (!(err = FSpGetFInfo(spec, &fndrInfo)))
		{
			if (!(perm & PG_BINARY))
				fndrInfo.fdType = 'TEXT';
			
			if (!(err = FSpSetFInfo(spec, &fndrInfo)))
			{
				SignedByte permission;
				
				switch (perm & PG_RDWR)
				{
					case PG_RDONLY:
						permission = fsRdPerm;
						break;
	
					case PG_WRONLY:
						permission = fsWrPerm;
						break;
	
					case PG_RDWR:
						permission = fsRdWrPerm;
						break;

					default:
						permission = fsCurPerm;
						break;
				}
				
				if (!(err = FSpOpenDF(spec, permission, ref_num)))
				{
					if (err == fnfErr && (perm & PG_CREAT))
						err = pgCreateFile(ref, perm, ref_num);
					
					if (!err)
					{
						if (perm & PG_APPEND)
							err = SetFPos(*ref_num, fsFromLEOF, 0);
						if (perm & PG_TRUNC)
							err = SetEOF(*ref_num, 0);
					}
				}
			}
		}
	}
	
	UnuseMemory(ref);
	
	return err;
}


/* Close a file */

PG_C (pg_error) pgCloseFile(pg_file_unit ref_num)
{
	return FSClose(ref_num);
}


/* Delete a file */

PG_C (pg_error) pgDeleteFile(pg_file_desc_ref ref)
{
	pg_error	err;
	
	err = FSpDelete((FSSpecPtr)UseMemory(ref));
	UnuseMemory(ref);
	
	return err;
}


/* Rename a file */

PG_C (pg_error) pgRenameFile(pg_file_desc_ref ref, const pg_file_name_ptr new_file_name)
{
	pg_error	err;
	
	err = FSpRename((FSSpecPtr)UseMemory(ref), new_file_name);
	UnuseMemory(ref);

	return err;
}


/* Create and open a temporary file */

PG_C (pg_file_desc_ref) pgOpenTempFile(const pgm_globals_ptr mem_globals, pg_file_unit_ptr temp_ref_num)
{
	FSSpec				spec;
	pg_error			err;
	pg_file_desc_ref	temp_ref = MEM_NULL;
	
	pgFailError(mem_globals, FindFolder(kOnSystemDisk, kTemporaryFolderType, kCreateFolder, &spec.vRefNum, &spec.parID));
	
	pgBlockMove("\pPaigeTmp", spec.name, 10);
	spec.name[++spec.name[0]] = '.';
	spec.name[++spec.name[0]] = 'A';
	
	while ((err = FSpCreate(&spec, PG_IO_CREATOR, PG_TEMP_TYPE, smSystemScript)) != NO_ERROR)
		spec.name[spec.name[0]]++;
	
	pgFailError(mem_globals, err);
	
	err = FSpOpenDF(&spec, fsRdWrPerm, temp_ref_num);
		
	pgFailError(mem_globals, err);

	temp_ref = MemoryAlloc (mem_globals, sizeof(FSSpec), 1, 0);
	*((FSSpecPtr)UseMemory(temp_ref)) = spec;
	UnuseMemory(temp_ref);
	
	return temp_ref;
}
		

#ifdef NO_C_INLINE

PG_C (pg_error) pgGetFileEOF(pg_file_unit ref_num, long PG_FAR *offset_result)
{
	return  GetEOF(ref_num, offset_result);
}


PG_C (pg_error) pgSetFileEOF(pg_file_unit ref_num, long offset)
{
	return  SetEOF(ref_num, offset);
}


PG_C (pg_error) pgGetFilePos(pg_file_unit ref_num, long PG_FAR *offset_result)
{
	return  GetFPos(ref_num, offset_result);
}


PG_C (pg_error) pgSetFilePos(pg_file_unit ref_num, long offset)
{
    return SetFPos(ref_num, fsFromStart, offset);
}


PG_C (pg_error) pgReadFileBytes(pg_file_unit ref_num, long PG_FAR *byte_size, void PG_FAR *buffer)
{
    return FSRead(ref_num, byte_size, buffer);
}


PG_C (pg_error) pgWriteFileBytes(pg_file_unit ref_num, long PG_FAR *byte_size, void PG_FAR *buffer)
{
    return FSWrite(ref_num, byte_size, buffer);
}

#endif


#endif

#ifdef POSIX_PLATFORM

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static size_t pg_posix_utf8_len(pg_char ch)
{
	unsigned long codepoint = (unsigned long)ch;

	if (codepoint < 0x80)
		return 1;
	if (codepoint < 0x800)
		return 2;
	return 3;
}

static char *pg_posix_path_from_pg_chars(const pg_file_name_ptr file_name)
{
	size_t bytes = 0;
	char *path;
	char *out;
	pg_char_ptr scan;

	if (!file_name)
		return NULL;

	for (scan = file_name; *scan; ++scan)
		bytes += pg_posix_utf8_len(*scan);

	path = (char *)malloc(bytes + 1);
	if (!path)
		return NULL;

	out = path;
	for (scan = file_name; *scan; ++scan) {
		unsigned long codepoint = (unsigned long)*scan;

		if (codepoint < 0x80)
			*out++ = (char)codepoint;
		else
		if (codepoint < 0x800) {
			*out++ = (char)(0xC0 | (codepoint >> 6));
			*out++ = (char)(0x80 | (codepoint & 0x3F));
		}
		else {
			*out++ = (char)(0xE0 | (codepoint >> 12));
			*out++ = (char)(0x80 | ((codepoint >> 6) & 0x3F));
			*out++ = (char)(0x80 | (codepoint & 0x3F));
		}
	}

	*out = 0;
	return path;
}

static pg_file_desc_ref pg_posix_path_to_descriptor(const pgm_globals_ptr mem_globals, const char *path)
{
	pg_file_desc_ref ref;
	size_t len;

	if (!path)
		return MEM_NULL;

	len = strlen(path) + 1;
	ref = MemoryAlloc(mem_globals, 1, len, 0);
	pgBlockMove((void PG_FAR *)path, UseMemory(ref), len);
	UnuseMemory(ref);

	return ref;
}

static int pg_posix_open_flags(short perm)
{
	int flags;

	switch (perm & PG_RDWR) {
		case PG_WRONLY:
			flags = O_WRONLY;
			break;

		case PG_RDWR:
			flags = O_RDWR;
			break;

		case PG_RDONLY:
		default:
			flags = O_RDONLY;
			break;
	}

	if (perm & PG_APPEND)
		flags |= O_APPEND;
	if (perm & PG_CREAT)
		flags |= O_CREAT;
	if (perm & PG_EXCL)
		flags |= O_EXCL;
	if (perm & PG_TRUNC)
		flags |= O_TRUNC;

	return flags;
}

PG_C (pg_file_desc_ref) pgFileSpec2FileDescriptor(const pgm_globals_ptr mem_globals, const pg_file_desc_ptr spec)
{
	return pg_posix_path_to_descriptor(mem_globals, spec);
}

PG_C (pg_file_desc_ref) pgCreateFileDescriptor(const pgm_globals_ptr mem_globals, const pg_file_name_ptr file_name)
{
	char *path = pg_posix_path_from_pg_chars(file_name);
	pg_file_desc_ref ref = pg_posix_path_to_descriptor(mem_globals, path);

	free(path);
	return ref;
}

PG_C (void) pgDisposeFileDescriptor(pg_file_desc_ref ref)
{
	DisposeNonNilFailedMemory(ref);
}

PG_C (pg_error) pgCreateFile(pg_file_desc_ref ref, short perm, pg_file_unit_ptr ref_num)
{
	short create_perm = (short)(perm | PG_CREAT);

	if (!(perm & PG_EXCL))
		create_perm |= PG_TRUNC;

	return pgOpenFile(ref, create_perm, ref_num);
}

PG_C (pg_error) pgOpenFile(pg_file_desc_ref ref, short perm, pg_file_unit_ptr ref_num)
{
	const char *path;
	int fd;
	int flags;
	const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

	if (!ref || !ref_num)
		return ACCESS_DENIED_ERR;

	path = (const char *)UseMemory(ref);
	flags = pg_posix_open_flags(perm);
	fd = open(path, flags, mode);
	UnuseMemory(ref);

	if (fd < 0)
		return (pg_error)errno;

	*ref_num = fd;
	return NO_ERROR;
}

PG_C (pg_error) pgCloseFile(pg_file_unit ref_num)
{
	return (close(ref_num) == 0) ? NO_ERROR : (pg_error)errno;
}

PG_C (pg_error) pgDeleteFile(pg_file_desc_ref ref)
{
	const char *path;
	int result;

	if (!ref)
		return ACCESS_DENIED_ERR;

	path = (const char *)UseMemory(ref);
	result = unlink(path);
	UnuseMemory(ref);

	return (result == 0) ? NO_ERROR : (pg_error)errno;
}

PG_C (pg_error) pgRenameFile(pg_file_desc_ref ref, const pg_file_name_ptr new_file_name)
{
	const char *old_path;
	char *new_path;
	int result;

	if (!ref || !new_file_name)
		return ACCESS_DENIED_ERR;

	old_path = (const char *)UseMemory(ref);
	new_path = pg_posix_path_from_pg_chars(new_file_name);
	result = new_path ? rename(old_path, new_path) : -1;
	UnuseMemory(ref);
	free(new_path);

	return (result == 0) ? NO_ERROR : (pg_error)(new_path ? errno : NO_MEMORY_ERR);
}

PG_C (pg_file_desc_ref) pgOpenTempFile(const pgm_globals_ptr mem_globals, pg_file_unit_ptr temp_ref_num)
{
	const char *tmpdir = getenv("TMPDIR");
	char path[1024];
	int fd;

	if (!tmpdir || !*tmpdir)
		tmpdir = "/tmp";

	snprintf(path, sizeof(path), "%s/paigeXXXXXX", tmpdir);
	fd = mkstemp(path);
	pgFailError(mem_globals, (fd < 0) ? (pg_error)errno : NO_ERROR);

	if (temp_ref_num)
		*temp_ref_num = fd;

	return pg_posix_path_to_descriptor(mem_globals, path);
}

PG_C (pg_error) pgGetFileEOF(pg_file_unit ref_num, size_t PG_FAR *offset_result)
{
	off_t current = lseek(ref_num, 0, SEEK_CUR);
	off_t end;

	if (current < 0)
		return (pg_error)errno;

	end = lseek(ref_num, 0, SEEK_END);
	if (end < 0)
		return (pg_error)errno;

	if (lseek(ref_num, current, SEEK_SET) < 0)
		return (pg_error)errno;

	if (offset_result)
		*offset_result = (size_t)end;

	return NO_ERROR;
}

PG_C (pg_error) pgSetFileEOF(pg_file_unit ref_num, size_t offset)
{
	return (ftruncate(ref_num, (off_t)offset) == 0) ? NO_ERROR : (pg_error)errno;
}

PG_C (pg_error) pgGetFilePos(pg_file_unit ref_num, size_t PG_FAR *offset_result)
{
	off_t offset = lseek(ref_num, 0, SEEK_CUR);

	if (offset < 0)
		return (pg_error)errno;

	if (offset_result)
		*offset_result = (size_t)offset;

	return NO_ERROR;
}

PG_C (pg_error) pgSetFilePos(pg_file_unit ref_num, size_t offset)
{
	return (lseek(ref_num, (off_t)offset, SEEK_SET) >= 0) ? NO_ERROR : (pg_error)errno;
}

PG_C (pg_error) pgReadFileBytes(pg_file_unit ref_num, size_t PG_FAR *byte_size, void PG_FAR *buffer)
{
	ssize_t bytes_read;

	if (!byte_size || !buffer)
		return ACCESS_DENIED_ERR;
	if (*byte_size == 0)
		return NO_ERROR;

	bytes_read = read(ref_num, buffer, *byte_size);
	if (bytes_read < 0)
		return (pg_error)errno;

	*byte_size = (size_t)bytes_read;
	return bytes_read == 0 ? EOF_ERR : NO_ERROR;
}

PG_C (pg_error) pgWriteFileBytes(pg_file_unit ref_num, size_t PG_FAR *byte_size, const void PG_FAR *buffer)
{
	size_t requested;
	size_t written_total = 0;
	const unsigned char *bytes = (const unsigned char *)buffer;

	if (!byte_size || !buffer)
		return ACCESS_DENIED_ERR;

	requested = *byte_size;
	while (written_total < requested) {
		ssize_t written = write(ref_num, bytes + written_total, requested - written_total);

		if (written < 0) {
			*byte_size = written_total;
			return (pg_error)errno;
		}
		if (written == 0)
			break;

		written_total += (size_t)written;
	}

	*byte_size = written_total;
	return (written_total == requested) ? NO_ERROR : IO_ERR;
}

#endif


#ifdef WINDOWS_PLATFORM

#include <stdio.h>


/* Create a file descriptor */

PG_C (pg_file_desc_ref) pgFileSpec2FileDescriptor(const pgm_globals_ptr mem_globals, const pg_file_desc_ptr spec)
{
	return CString2Memory (mem_globals, (pg_c_string_ptr)spec);
}


/* Create a file descriptor */

PG_C (pg_file_desc_ref) pgCreateFileDescriptor(const pgm_globals_ptr mem_globals, const pg_file_name_ptr file_name)
{	
	return CString2Memory (mem_globals, file_name);
}


#ifdef NO_C_INLINE

/* Dispose of a file descriptor */

PG_C (void) pgDisposeFileDescriptor(pg_file_desc_ref ref)
{
	DisposeNonNilFailedMemory(ref);
}

#endif


// create a file
PG_C (pg_error) pgCreateFile(pg_file_desc_ref ref, short perm, pg_file_unit_ptr ref_num)
{
	pg_error	err = NO_ERROR;
	
	if ((*ref_num = _lcreat((LPCSTR)UseMemory(ref), 0)) == HFILE_ERROR)
	{
		err = (pg_error)HFILE_ERROR;
	}
	UnuseMemory(ref);
	
	return err;
}


// open a file
PG_C (pg_error) pgOpenFile(pg_file_desc_ref ref, short perm, pg_file_unit_ptr ref_num)
{
	pg_error	err = NO_ERROR;
	
	if ((*ref_num = _lopen((LPCSTR)UseMemory(ref), perm)) == HFILE_ERROR)
	{
		err = (pg_error)HFILE_ERROR;
	}
	UnuseMemory(ref);

	return err;
}


// close a file
PG_C (pg_error) pgCloseFile(pg_file_unit ref_num)
{
	pg_error	err = NO_ERROR;
	
	if (_lclose(ref_num) == HFILE_ERROR)
	{
		err = (pg_error)HFILE_ERROR;
	}
	return err;
}


// delete a file
PG_C (pg_error) pgDeleteFile(pg_file_desc_ref ref)
{
	pg_error	err = NO_ERROR;
	
	if (remove((LPCSTR)UseMemory(ref)) == HFILE_ERROR)
	{
		err = (pg_error)HFILE_ERROR;
	}
	UnuseMemory(ref);

	return err;
}


// rename a file
PG_C (pg_error) pgRenameFile(pg_file_desc_ref ref, const pg_file_name_ptr new_file_name)
{
	pg_error	err = NO_ERROR;
	
	if (rename((LPCSTR)UseMemory(ref), (LPCSTR)new_file_name) == HFILE_ERROR)
	{
		err = (pg_error)HFILE_ERROR;
	}
	UnuseMemory(ref);

	return err;
}


// open temporary a file
PG_C (pg_file_desc_ref) pgOpenTempFile(const pgm_globals_ptr mem_globals, pg_file_unit_ptr temp_ref_num)
{
	pg_file_name_ptr	p;
	long				len;
	pg_error			err = NO_ERROR;
	short				cnt = 200;
	pg_file_desc_ref	temp_ref = MEM_NULL;
	
	p = (pg_file_name_ptr)tmpnam(NULL);
	while ((*temp_ref_num = _lopen((LPCSTR)p, 0 /*����*/)) == HFILE_ERROR)
	{
		if (!--cnt)
			break;
	}
	if (*temp_ref_num == HFILE_ERROR)
	{
		err = (pg_error)HFILE_ERROR;
	}
	
	pgFailError(mem_globals, err);

	len = pgCStrLength(p) + 1;
	temp_ref = MemoryAlloc (mem_globals, sizeof(pg_char), len, 0);
	pgBlockMove(p, (pg_bits8_ptr)UseMemory(temp_ref), len * sizeof(pg_char));
	UnuseMemory(temp_ref);
	
	return temp_ref;
}


#ifdef NO_C_INLINE

PG_C (pg_error) pgGetFileEOF(pg_file_unit ref_num, long PG_FAR *offset_result)
{
	if ((*offset_result = _llseek(ref_num, 0L, SEEK_END)) == EOF)
		return EOF;
    return NO_ERROR;
}




PG_C (pg_error) pgGetFilePos(pg_file_unit ref_num, long PG_FAR *offset_result)
{
	if ((*offset_result = _llseek(ref_num, 0L, SEEK_CUR)) == EOF)
		return EOF;
    return NO_ERROR;
}


PG_C (pg_error) pgSetFilePos(pg_file_unit ref_num, long offset)
{
	if (_llseek(ref_num, offset, SEEK_SET) == EOF)
		return EOF;
    return NO_ERROR;
}


#endif

PG_C (pg_error) pgSetFileEOF(pg_file_unit ref_num, long offset)
{
	pg_error err;

	err = pgSetFilePos(ref_num, offset);
	if (err == NO_ERROR)
		err = _lwrite(ref_num, (LPCCH)&err, 0);

    return err;
}


PG_C (pg_error) pgWriteFileBytes(pg_file_unit ref_num, long PG_FAR *byte_size, void PG_FAR *buffer)
{
	long		byte_count = *byte_size;
	pg_error	err = NO_ERROR;
	
	byte_count = _hwrite(ref_num, (LPCCH) buffer, byte_count);
	if (*byte_size != byte_count)
	{
		return (pg_error)HFILE_ERROR;
	}

	*byte_size = byte_count;

    return err;
}


PG_C (pg_error) pgReadFileBytes(pg_file_unit ref_num, long PG_FAR *byte_size, void PG_FAR *buffer)
{
	long	byte_count = *byte_size;
	
	byte_count = _hread(ref_num, buffer, byte_count);
	*byte_size = byte_count;
	if (byte_count <= 0)
	{
		if (!byte_count)
			return EOF_ERR;
		else return (pg_error)HFILE_ERROR;
	}
    return NO_ERROR;
}


#endif


PG_C (pg_error) pgReadFileData(pg_file_unit ref_num, size_t byte_size, void PG_FAR *buffer)
{
	size_t	byte_count = byte_size;
	
	return pgReadFileBytes(ref_num, &byte_count, buffer);
}


PG_C (pg_error) pgWriteFileData(pg_file_unit ref_num, size_t byte_size, const void PG_FAR *buffer)
{
	size_t	byte_count = byte_size;
	
	return pgWriteFileBytes(ref_num, &byte_count, (void PG_FAR *)buffer);
}


PG_C (void) pgSetTypeCreator(const pgm_globals_ptr mem_globals, long creator, long fileType)
{
    mem_globals->creator = creator;
    mem_globals->fileType = fileType;
}



/* This is a "fake" file I/O proc that sends the data to a memory_ref instead of
a file.	*/

PG_PASCAL (pg_error) pgScrapMemoryWrite (void PG_FAR *data, short verb, size_t PG_FAR *position,
		size_t PG_FAR *data_size, file_ref filemap)
{
	pg_bits8_ptr		new_data, source_data;
	size_t				ref_size;

	if (verb == io_set_fpos)
		return	NO_ERROR;
	
	if (verb == io_set_eof) {

		SetMemorySize(filemap, *position);
		return	NO_ERROR;
	}

	if (verb == io_data_indirect)
		source_data = (pg_bits8_ptr) UseMemory((memory_ref) data);
	else
		source_data = (pg_bits8_ptr) data;
	
	ref_size = GetMemorySize(filemap);

	if (ref_size > *position) {
		
		if ((*position + *data_size) > ref_size)
			SetMemorySize(filemap, *position + *data_size);

		new_data = (pg_bits8_ptr) UseMemoryRecord(filemap, *position, USE_ALL_RECS, TRUE);
	}
	else
		new_data = (pg_bits8_ptr) AppendMemory(filemap, *data_size, FALSE);

	pgBlockMove(source_data, new_data, *data_size);
	UnuseMemory(filemap);
	
	if (verb == io_data_indirect)
		UnuseMemory((memory_ref)data);

	*position += *data_size;

	return	NO_ERROR;
}



/* pgScrapMemoryRead is a "fake" file I/O proc that reads the data into a memory_ref instead of
a file.  */

PG_PASCAL (pg_error) pgScrapMemoryRead (void PG_FAR *data, short verb, size_t PG_FAR *position,
		size_t PG_FAR *data_size, file_ref filemap)
{
	size_t PG_FAR		*ptr_to_long;

	if (verb == io_set_fpos)
		return	NO_ERROR;
	
	if (verb == io_file_unit) {
		
		ptr_to_long = (size_t PG_FAR *)data;
		*ptr_to_long = (size_t)filemap;
		
		*((pg_file_unit PG_FAR *)data) = (pg_file_unit)filemap;
		return	NO_ERROR;
	}
	else
	if (verb == io_get_eof) {
		
		ptr_to_long = (size_t PG_FAR *)data;
		*ptr_to_long = GetMemorySize(filemap);
	}
	else {
		pg_bits8_ptr	the_data, target_data;
		
		the_data = (pg_bits8_ptr) UseMemory(filemap);
		the_data += *position;
		
		if (verb == io_data_indirect) {
			
			SetMemorySize((memory_ref) data, *data_size);
			target_data = (pg_bits8_ptr) UseMemory((memory_ref) data);
		}
		else
			target_data = (pg_bits8_ptr) data;

		pgBlockMove(the_data, target_data, *data_size);
		UnuseMemory(filemap);
		
		if (verb == io_data_indirect)
			UnuseMemory((memory_ref)data);

		*position += *data_size;
	}
	
	return	NO_ERROR;
}



