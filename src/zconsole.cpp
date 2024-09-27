#include "core.h"
#include "zconsole.h"

#include <iostream>
#include <filesystem>
#include <string>
#include <codecvt>
#include <iomanip>

namespace fs = std::filesystem;

ZConsole::ZConsole(const std::filesystem::path &path)
{
    std::vector<std::string> _paths = zutils::splitPath(path.string());
    this->curPaths = {};
    this->root = std::make_shared<ZFdNode>(_paths[0]);
    this->curNode = this->root;
    this->cd(path);
}

ZConsole::~ZConsole()
{
    this->root.reset();
}

std::vector<std::string> ZConsole::destination(Paths_Ty &paths)
{

    Paths_Ty newPaths = this->curPaths;
    for (auto &path : paths)
    {
        if (path == "..")
            newPaths.pop_back();
        else
            newPaths.push_back(path);
    }
    // verify
    std::filesystem::path newPath = zutils::joinPath(newPaths);
    bool is = std::filesystem::is_directory(newPath);
    if (!is)
    {
        throw std::runtime_error("Not a valid destination directory: " + newPath.string());
    }
    return newPaths;
}

void ZConsole::cd(const std::filesystem::path &path)
{
#if ZQ_INFO
    std::cout << "[info] " << "Will change to directory: " << path.string() << std::endl;
#endif
    Paths_Ty path_through;
    if (path.is_absolute())
    {
        path_through = zutils::splitPath(path.string());
    }
    else if (path.is_relative())
    {
        auto _split = zutils::splitPath(path.string());
        path_through = this->destination(_split);
    }
    else
    {
        throw std::runtime_error("Not a valid path: " + path.string());
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
    auto cur = this->root;
    for (size_t i = 0; i < path_through.size(); i++)
    {
        cur = cur.get()->locate(path_through[i]);
    }
    this->curNode = cur;
}

void ZConsole::scan()
{
    this->curNode.get()->scan();
}

void ZConsole::info()
{
    std::cout<< this->curNode.get()->print() << std::endl;
}

void ZConsole::PROMPTING(bool enter)
{
    if (enter)
    {
        std::cout << std::endl;
    }
    std::cout << "[" << this->curNode.get()->path() << "]> ";
}


void ZConsole::ls()
{
    size_t _cnt = 0;
    std::cout << std::endl;
    for (const auto &entry : fs::directory_iterator(this->curNode.get()->path()))
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
