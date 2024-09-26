#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

struct FolderInfo
{
    std::string name;
    std::vector<std::string> path;
    uintmax_t size;
    int children_count;
    time_t last_modified;
    bool less_than_5mb;

    FolderInfo *parent;                 
    std::vector<FolderInfo *> children;

    FolderInfo(const std::filesystem::path &dir_path);
    ~FolderInfo();
    std::string info();
};

FolderInfo *scan_folder(const std::filesystem::path &dir_path) ;
FolderInfo *scan_folder(const std::filesystem::path &dir_path, FolderInfo *parent);