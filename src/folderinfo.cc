
#include "prompt.h"

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <chrono>

ZFolderInfo::ZFolderInfo(const std::string &dir_name, ZFolderInfo *parent): parent(parent), name(dir_name)
{
#ifdef ZQ_DEBUG
    std::cout << "[debug] [FolderInfo] [contructor]: " << this->path().string() << std::endl;
#endif
    size = 0;
    last_modified = 0;
    fully_scanned = false;
    childrenCount = 0;
    depth = parent->depth + 1;
    children = {};
}
ZFolderInfo::ZFolderInfo(const std::string &dir_name): parent(nullptr), name(dir_name)
{
#ifdef ZQ_DEBUG
    std::cout << "[debug] [FolderInfo] [contructor]: " << dir_name << std::endl;
#endif
    size = 0;
    last_modified = 0;
    fully_scanned = false;
    childrenCount = 0;
    depth = -1;
    children = {};
}

void ZFolderInfo::reset()
{
    this->size = 0;
    this->last_modified = zq_fswalk::getLastModified(this->path());
    this->fully_scanned = false;
    this->childrenCount = 0;
    this->depth = this->parent->depth + 1;
    for (auto child : children)
    {
        child->_scanned = false; // reset flag
    }
}

ZFolderInfo::~ZFolderInfo()
{
    for (auto child : children)
        delete child;
}

std::string ZFolderInfo::info()
{
    std::string builder;
    builder += "name: " + name + "\n";
    builder += "path: " + path().string() + "\n";
    builder += "size: " + std::to_string(double(size) / 1024 / 1024) + "MB\n";
    builder += "last_modified: " + std::to_string(last_modified) + "\n";
    builder += "children_count: " + std::to_string(childrenCount) + "\n";
    builder += "depth: " + std::to_string(depth) + "\n";
    return builder;
}

ZFolderInfo *ZFolderInfo::find_tree(const std::vector<std::string> &names, int depth)
{
#ifdef ZQ_DEBUG
    std::cout << "[debug] [FolderInfo::find_tree] " << this->name << " find_tree: " << names[depth] << " at depth: " << depth << std::endl;
#endif
    if (depth > names.size() - 1) 
    {
        throw std::runtime_error("[FolderInfo::find_tree] depth out of range");
    }
    for (auto child : this->children)
    {
        if (child->name == names[depth]) {
            if (depth == names.size() - 1) {
#ifdef ZQ_DEBUG
    std::cout << "[debug] [FolderInfo::find_tree] " << this->name << " founded: " << names[depth] << " at depth: " << depth << std::endl;
#endif
                return child;
            }
            return child->find_tree(names, depth + 1);
        }
    }
    return nullptr;
}

ZFolderInfo *ZFolderInfo::find_children(const std::string &_name)
{
    for (auto child : children)
        if (child->name == _name)
            return child;
    return nullptr;
}

std::filesystem::path ZFolderInfo::path()
{
    std::string builder = this->name;
    auto _parent = this->parent;
    while (_parent != nullptr)
    {
        builder = _parent->name + "/" + builder;
        _parent = _parent->parent;
    }
    return builder;
}

void ZFolderInfo::scan()
{
    if (!this->verify())
    {
        this->reset();
        for (const auto &entry : std::filesystem::directory_iterator(this->path()))
        {
            if (entry.is_directory())
            {
                ZFolderInfo *info = this->find_children(entry.path().filename().string());
                if (info != nullptr && info->verify())
                {
                    this->size += info->size;
                    continue;
                }
                uintmax_t infoSize = zq_fswalk::getFolderSize(entry.path());
                this->size += infoSize;

                if (infoSize > 5 * 1024 * 1024) {
                    if (info == nullptr)
                    {
                        this->children.push_back(new ZFolderInfo(entry.path().filename().string(), this));
                        this->children.back()->reset();
                        this->children.back()->fully_scanned = true;
                        this->children.back()->size = infoSize;
                    }
                    else
                    {
                        info->reset();
                        info->fully_scanned = true;
                        info->size = infoSize;
                    }
                }
            }
            else if (entry.is_regular_file())
            {
                this->size += std::filesystem::file_size(entry.path());
            }
            this->childrenCount++;
        }
        for (auto it = this->children.begin(); it != this->children.end(); it++)
            if (!(*it)->_scanned)
            {
                this->children.erase(it);
            }
    }
    this->fully_scanned = true;
}

inline bool ZFolderInfo::verify()
{
    return zq_fswalk::getLastModified(this->path()) == this->last_modified && fully_scanned;
}

