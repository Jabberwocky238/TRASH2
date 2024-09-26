#pragma once
#include "fswalk.h"

struct Console
{
    FolderInfo *cur_info;
    FolderInfo *root;
    std::vector<std::string> curPaths;
    
    Console(const std::filesystem::path &path);
    ~Console();

    std::filesystem::path destination(std::vector<std::string> &paths) const;
    void cd(const std::filesystem::path &path);
    void ls();
    void scan();
};
