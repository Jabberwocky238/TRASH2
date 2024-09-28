#pragma once
#include "zfs/fdnode.h"


#include <filesystem>
#include <string>
#include <vector>

struct ZConsole
{
    using Paths_Ty = std::vector<std::string>;

    std::shared_ptr<ZFdNode> root;
    Paths_Ty curPaths;
    std::shared_ptr<ZFdNode> curNode;
    
    ZConsole(const std::filesystem::path &path);
    ~ZConsole();

    Paths_Ty destination(Paths_Ty &paths);
    void cd(const std::filesystem::path &path);
    void ls();
    void scan();

    void info();
    void PROMPTING(bool enter = true);
};