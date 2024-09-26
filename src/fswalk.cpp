#include "fswalk.h"

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <chrono>

namespace fs = std::filesystem;
namespace _fswalk
{
    inline const time_t last_modified(const fs::path &path)
    {
        fs::file_time_type ftime = fs::last_write_time(path);
        time_t cftime = std::chrono::system_clock::to_time_t(
            std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
            )
        );
        return cftime;
    }
    inline const std::vector<std::string> split_path(const std::string &path)
    {
        std::vector<std::string> builder;
        std::string part;
        for (auto c : path) {
            if (c == '/') {
                builder.push_back(part);
                part = "";
            } else {
                part += c;
            }
        }
        return builder;
    };
    inline const std::string join_path(const std::vector<std::string> &path)
    {   
        std::string builder;
        for (auto part : path) {
            builder += part + "/";
        }
        return builder;
    };
    // inline const std::wstring path_to_wstring(const fs::path &path)
    // {
    //     return path.filename().wstring();
    // };
    // inline const std::string wstring_to_utf8(const std::wstring &wstr)
    // {
    //     std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    //     return conv.to_bytes(wstr);
    // };
}

FolderInfo::FolderInfo(const std::filesystem::path &dir_path)
{
    name = dir_path.filename().string();
    path = _fswalk::split_path(dir_path.string());
    size = fs::file_size(dir_path);
    last_modified = _fswalk::last_modified(dir_path);
    children_count = 0;
    less_than_5mb = false;
}

FolderInfo::~FolderInfo()
{
    for (auto child : children) {
        delete child;
    }
}
std::string FolderInfo::info() {
    std::string builder;
    builder += "name: " + name + "\n";
    builder += "path: " + _fswalk::join_path(path) + "\n";
    builder += "size: " + std::to_string( double(size) / 1024 / 1024 ) + "MB\n";
    builder += "last_modified: " + std::to_string(last_modified) + "\n";
    builder += "children_count: " + std::to_string(children_count) + "\n";
    return builder;
}

FolderInfo *scan_folder(const fs::path &dir_path) 
{
    FolderInfo *info = new FolderInfo(dir_path);
    return scan_folder(dir_path, info);
}

FolderInfo *scan_folder(const fs::path &dir_path, FolderInfo *parent)
{
    for (const auto &entry : fs::directory_iterator(dir_path))
    {
        if (entry.is_directory())
        {
            FolderInfo *info = scan_folder(entry.path());
            parent->children.push_back(info);
            parent->children_count++;
            parent->size += info->size;
            parent->less_than_5mb = parent->size < 5 * 1024 * 1024;
        }
        else if (entry.is_regular_file())
        {
            parent->size += fs::file_size(entry);
            parent->children_count++;
            parent->less_than_5mb = parent->size < 5 * 1024 * 1024;
        }
    }
    return parent;
}

