#include "FileCollector.h"

namespace fs = std::filesystem;

static bool is_source_file(const fs::path& p) {
    std::string ext = p.extension().string();
    return ext == ".h" || ext == ".hpp" ||
           ext == ".c" || ext == ".cpp";
}

std::vector<fs::path>
FileCollector::collect(const fs::path& root) {
    std::vector<fs::path> files;

    for(const auto& entry :
         fs::recursive_directory_iterator(
             root,
             fs::directory_options::skip_permission_denied)) {

        if(entry.is_regular_file() && is_source_file(entry.path())) {
            files.push_back(entry.path());
        }
    }

    return files;
}

