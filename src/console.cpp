#include "prompt.h"

#include <iostream>
#include <filesystem>
#include <string>
#include <codecvt>
#include <iomanip>

namespace fs = std::filesystem;

ZConsole::ZConsole(const std::filesystem::path &path)
{
    std::vector<std::string> _paths = zq_fswalk::splitPath(path.string());
    this->curPaths = {};
    this->root = new ZFolderInfo(_paths[0]);
    this->root->depth = 0;
    this->cur_info = nullptr;
    this->cd(path);
}

ZConsole::~ZConsole()
{
    delete this->root;
}

std::filesystem::path ZConsole::destination(std::vector<std::string> &paths) const
{

    std::vector<std::string> newPaths = this->curPaths;
    for (auto &path : paths)
    {
        if (path == "..")
            newPaths.pop_back();
        else
            newPaths.push_back(path);
    }
    // verify
    std::filesystem::path newPath = zq_fswalk::joinPath(newPaths);
    bool is = std::filesystem::is_directory(newPath);
    if (!is) {
        throw std::runtime_error("Not a valid destination directory: " + newPath.string());
    }
    return newPath;
}

void ZConsole::cd(const std::filesystem::path &path)
{
    try
    {   
        std::cout << "[info] " << "Will change to directory: " << path.string() << std::endl;
        std::vector<std::string> path_through;
        if(path.is_absolute()) 
        {
            path_through = zq_fswalk::splitPath(path.string());
        }
        else if (path.is_relative())
        {
            path_through = zq_fswalk::splitPath(path.string());
            std::filesystem::path newPath = this->destination(path_through);
#ifdef ZQ_DEBUG
    std::cout << "[debug] newPath: " << newPath.string() << std::endl;
#endif
            path_through = zq_fswalk::splitPath(newPath.string());
        }
        this->curPaths = path_through;
        path_through.erase(path_through.begin()); // erase the disk symbol
#ifdef ZQ_DEBUG
    std::cout << "[debug] path: ";
    for (auto &path : path_through)
    {
        std::cout << path << " - ";
    }
    std::cout << std::endl;
#endif
        if (this->cur_info != nullptr)
        {
            // not null and found
            auto *_to = this->root->find_tree(path_through, 0);
            if (_to != nullptr)
            {
                this->cur_info = _to;
                return;
            }
        }
        std::cout << "[info] " << "Not found pointer, Need to build new folder info" << std::endl;

        ZFolderInfo *info = this->root;
        
        for (auto &name : path_through)
        {
            auto *child = info->find_children(name);
            if (child == nullptr)
            {
                child = new ZFolderInfo(name, info);
                child->reset();
                info->children.push_back(child);
#ifdef ZQ_DEBUG
    std::cout << "[debug] new child: " << name << std::endl;
#endif
            }
            info = child;
        }
        this->cur_info = info;
        return;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return;
}

void ZConsole::scan()
{
    this->cur_info->scan();
}

void ZConsole::PROMPTING(bool enter)
{
    if (enter) {
        std::cout << std::endl;
    }
    std::cout << "[" << this->cur_info->path() << "]> ";
}

void ZConsole::ls()
{
    size_t _cnt = 0;
    std::cout << std::endl;
    for (const auto &entry : fs::directory_iterator(this->cur_info->path()))
    {
        std::string name = entry.path().filename().string();
        if (entry.is_directory())
        {
            name += "/";
        }
        size_t _indent = (name.length() / 20 + 1);
        std::cout << std::left << std::setw(_indent * 20) << name;

        _cnt += _indent > 4 ? 4 : _indent;
        if (_cnt % 4 == 0)
        {
            std::cout << std::endl;
        }
    }
}


