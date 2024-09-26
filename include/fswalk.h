#pragma once
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

// #define ZQ_DEBUG

namespace zq_fswalk
{
    time_t get_last_modified(const std::filesystem::path &path);
    std::vector<std::string> split_path(const std::string &path);
    std::filesystem::path join_path(const std::vector<std::string>::iterator &begin, const std::vector<std::string>::iterator &end);
}

struct FolderInfo
{
private:
    std::string name;

    uintmax_t size;
    time_t last_modified;
    bool less_than_5mb;
    bool fully_scanned;
    bool _scanned;

    int children_count;
    int dir_count;
    int file_count;

public:
    FolderInfo *parent;
    std::vector<FolderInfo *> children;

    FolderInfo(const std::string &dir_name, FolderInfo *_parent);
    ~FolderInfo();

    std::string info();
    FolderInfo *root();
    FolderInfo *find_tree(const std::vector<std::string> &names, int depth);
    FolderInfo *find_children(const std::string &name);
    std::filesystem::path path();
    void scan();
    bool verify();
    void reset();
};
