#include "core.h"

#include <filesystem>
#include <string>
#include <vector>
#include <chrono>

namespace zutils
{
    time_t getLastModified(const std::filesystem::path &path);
    uintmax_t getFolderSize(const std::filesystem::path &path);
    std::vector<std::string> splitPath(const std::string &path);
    std::filesystem::path joinPath(const std::vector<std::string>& paths);
    std::vector<std::string> splitString(const std::string &str, char delimiter);
}