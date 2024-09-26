#pragma once
#include "fswalk.h"

struct Console
{
    FolderInfo *cur_info;
    std::filesystem::path cur_path;
    Console(const std::filesystem::path &path);
    ~Console();

    bool cd(const std::filesystem::path &path);
    FolderInfo *scan();
};