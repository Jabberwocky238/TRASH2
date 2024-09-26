#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

struct ZFolder
{
    ZFolder();
    ~ZFolder();

    ZFolder* parent;
    std::vector<ZFolder*> children;
    
    std::string name;
    time_t lastModified;
    uintmax_t size;
    int childrenCount;
    int depth;

    std::string info();
    std::filesystem::path path();
    // FolderInfo *root();
    // FolderInfo *find_tree(const std::vector<std::string> &names, int depth);
    // FolderInfo *find_children(const std::string &name);
    void scan();
    bool verify();
};


struct ZConsole
{
    std::vector<std::string> paths;
    ZFolder* current;
    ZFolder* root;

    void cd(const std::vector<std::string>&paths);
    void ls();
    void scan();
};

time_t getLastModified(const std::filesystem::path& path);
uintmax_t getFolderSize(const std::filesystem::path& path);
std::vector<std::string> splitPath(const std::string &path);
std::filesystem::path joinPath(const std::vector<std::string> &path);