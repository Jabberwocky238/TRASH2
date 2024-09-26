#include "prompt.h"

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <chrono>

namespace fs = std::filesystem;

namespace zq_fswalk
{
    time_t getLastModified(const fs::path &path)
    {
        fs::file_time_type ftime = fs::last_write_time(path);
        time_t cftime = std::chrono::system_clock::to_time_t(
            std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()));
        return cftime;
    }
    uintmax_t getFolderSize(const std::filesystem::path &path)
    {
        uintmax_t size = 0;
        for (const auto &entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory())
            {
                size += getFolderSize(entry.path());
            }
            else if(entry.is_regular_file())
            {
                size += entry.file_size();
            }
        }
        return size;
    }
    std::vector<std::string> splitPath(const std::string &path)
    {
        std::vector<std::string> builder;
        std::string part;
        for (auto c : path)
        {
            if (c == '/' || c == '\\')
            {
                builder.push_back(part);
                part = "";
            }
            else
            {
                part += c;
            }
        }
        if (part.size() > 0) builder.push_back(part);
        return builder;
    };
    std::filesystem::path joinPath(const std::vector<std::string>& paths)
    {
        std::string builder;
        for (auto path : paths)
        {
            builder += path + "/";
        }
        return builder;
    };
}