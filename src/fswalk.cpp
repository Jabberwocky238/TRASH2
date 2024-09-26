#include "fswalk.h"

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <chrono>

namespace fs = std::filesystem;

namespace zq_fswalk
{
    time_t get_last_modified(const fs::path &path)
    {
        fs::file_time_type ftime = fs::last_write_time(path);
        time_t cftime = std::chrono::system_clock::to_time_t(
            std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()));
        return cftime;
    }
    std::vector<std::string> split_path(const std::string &path)
    {
        std::vector<std::string> builder;
        std::string part;
        for (auto c : path)
        {
            if (c == '/' || c == '\\')
            {
                builder.push_back(part);
                part = "";
            }
            else
            {
                part += c;
            }
        }
        builder.push_back(part);
        return builder;
    };
    std::filesystem::path join_path(const std::vector<std::string>::iterator &begin, const std::vector<std::string>::iterator &end)
    {
        std::string builder;
        for (auto it = begin; it != end; it++)
        {
            builder += *it + "\\";
        }
        return builder;
    };
}

FolderInfo::FolderInfo(const std::string &dir_name)
{
    name = dir_name;
    std::cout << "[info] " << "build: " << dir_name << std::endl;
    last_modified = 0;

    size = 0;
    less_than_5mb = false;
    fully_scanned = false;

    children = {};
    children_count = 0;
    dir_count = 0;
    file_count = 0;
    parent = nullptr;
}

FolderInfo::~FolderInfo()
{
    for (auto child : children)
        delete child;
}

std::string FolderInfo::info()
{
    std::string builder;
    builder += "name: " + name + "\n";
    builder += "path: " + path().string() + "\n";
    builder += "size: " + std::to_string(double(size) / 1024 / 1024) + "MB\n";
    builder += "last_modified: " + std::to_string(last_modified) + "\n";
    builder += "children_count: " + std::to_string(children_count) + "\n";
    return builder;
}

FolderInfo *FolderInfo::root()
{
    auto *root = this;
    while (root->parent != nullptr)
        root = root->parent;
    return root;
    // return path.stem();
}

FolderInfo *FolderInfo::find_tree(const std::vector<std::string> &names, int depth)
{
    std::cout << "[info] " << this->name << " find_tree: " << names[depth] << std::endl;
    for (auto child : children)
    {
        if (child->name == names[depth])
            return child;
        else {
            if (depth == names.size() - 1)
                return nullptr;
            else
                return child->find_tree(names, depth + 1);
        }
    }
    return nullptr;
}

FolderInfo *FolderInfo::find_children(const std::string &_name)
{
    for (auto child : children)
        if (child->name == _name)
            return child;
    return nullptr;
}

std::filesystem::path FolderInfo::path()
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

void FolderInfo::scan()
{
    if (!(fully_scanned && this->verify()))
    {
        this->reset();
        for (const auto &entry : fs::directory_iterator(this->path()))
        {
            if (entry.is_directory())
            {
                FolderInfo *info = this->find_children(entry.path().filename().string());
                if (info == nullptr)
                {
                    info = new FolderInfo(entry.path().filename().string());
                    info->parent = this;
                    this->children.push_back(info);
                }
                info->scan();          // scan again
                info->_scanned = true; // set flag
                this->dir_count++;
                this->size += info->size;
            }
            else if (entry.is_regular_file())
            {
                this->file_count++;
                this->size += fs::file_size(entry.path());
            }
            this->children_count = this->dir_count + this->file_count;
            this->less_than_5mb = this->size < 5 * 1024 * 1024;
        }
        for (auto it = this->children.begin(); it != this->children.end(); it++)
            if (!(*it)->_scanned)
            {
                this->children.erase(it);
            }
    }
    this->fully_scanned = true;
}

inline bool FolderInfo::verify()
{
    return zq_fswalk::get_last_modified(this->path()) == this->last_modified;
}

void FolderInfo::reset()
{
    last_modified = zq_fswalk::get_last_modified(this->path());

    size = 0;
    less_than_5mb = false;
    fully_scanned = false;

    children_count = 0;
    dir_count = 0;
    file_count = 0;

    for (auto child : children)
    {
        child->_scanned = false; // reset flag
    }
}
