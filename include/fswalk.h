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

struct FolderInfo
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


    FolderInfo *parent;
    std::vector<FolderInfo *> children;

    FolderInfo(const std::string &dir_name);
    FolderInfo(const std::string &dir_name, FolderInfo *parent);
    ~FolderInfo();

    std::string info();
    // FolderInfo *root();
    FolderInfo *find_tree(const std::vector<std::string> &names, int depth);
    FolderInfo *find_children(const std::string &name);
    std::filesystem::path path();
    void scan();
    bool verify();
    void reset();
};
