#include "Paige.h"
#include "pgMemMgr.h"
#include "pgtxrcpp.h"

class ExportHandler {
public:
    ExportHandler(pg_ref pg) : paigeDoc(pg) {}

    bool ExportToASCII(const std::string& filePath) {
        // Implement ASCII export logic
        return Export(filePath, pg_text_type);
    }

    bool ExportToPaigeFormat(const std::string& filePath) {
        // Implement HERMES Paige format export logic
        return Export(filePath, pg_paige_type);
    }

    bool ExportToRTF(const std::string& filePath) {
        // Implement RTF export logic
        return Export(filePath, pg_rtf_type);
    }

private:
    pg_ref paigeDoc;

    bool Export(const std::string& filePath, pg_filetype fileType) {
        // Common export logic for different file types
        pg_file_unit fileUnit = pgOpenFile(filePath.c_str(), "wb");
        if (!fileUnit) {
            return false;
        }

        pg_error result = pgExportFile(paigeDoc, fileType, fileUnit, 0, 0, 0);
        pgCloseFile(fileUnit);

        return result == NO_ERROR;
    }
};
