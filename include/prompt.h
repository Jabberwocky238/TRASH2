#pragma once
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

#define ZQ_DEBUG

namespace zq_fswalk
{
    time_t getLastModified(const std::filesystem::path &path);
    uintmax_t getFolderSize(const std::filesystem::path &path);
    std::vector<std::string> splitPath(const std::string &path);
    std::filesystem::path joinPath(const std::vector<std::string> &path);
}

struct ZFolderInfo
{
private:
    std::string name;

    uintmax_t size;
    time_t last_modified;
    bool fully_scanned;
    bool _scanned;
public:
    int childrenCount;
    int depth;

    ZFolderInfo *parent;
    std::vector<ZFolderInfo *> children;

    ZFolderInfo(const std::string &dir_name);
    ZFolderInfo(const std::string &dir_name, ZFolderInfo *parent);
    ~ZFolderInfo();

    std::string info();
    // FolderInfo *root();
    ZFolderInfo *find_tree(const std::vector<std::string> &names, int depth);
    ZFolderInfo *find_children(const std::string &name);
    std::filesystem::path path();
    void scan();
    bool verify();
    void reset();
};

struct ZConsole
{
    ZFolderInfo *cur_info;
    ZFolderInfo *root;
    std::vector<std::string> curPaths;
    
    ZConsole(const std::filesystem::path &path);
    ~ZConsole();

    std::filesystem::path destination(std::vector<std::string> &paths) const;
    void cd(const std::filesystem::path &path);
    void ls();
    void scan();

    void PROMPTING(bool enter = true);
};


void prompt();