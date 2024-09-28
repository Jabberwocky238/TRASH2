#pragma once
#include "zcore/fdnode.h"
#include "zcore/zutils.h"

#include <filesystem>
#include <string>
#include <vector>

struct ZConsole
{
    using Paths_Ty = std::vector<std::string>;

    std::shared_ptr<ZFdNode> root;
    std::shared_ptr<ZFdNode> curNode;
    Paths_Ty curPaths;
    
    ZConsole();
    ~ZConsole();

    std::filesystem::path destination(std::filesystem::path &paths);
    void cd(std::filesystem::path &path);
    void ls();
    void scan();

    void info();
    void PROMPTING(bool enter = false);

    static void run();
};