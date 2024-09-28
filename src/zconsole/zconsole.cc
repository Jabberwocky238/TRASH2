#include "core.h"
#include "zconsole.h"
#include "zcore/zutils.h"

#include <iostream>
#include <filesystem>
#include <string>
#include <codecvt>
#include <iomanip>

namespace fs = std::filesystem;

ZConsole::ZConsole()
{
    auto path = fs::current_path();
    this->curPaths = {};
    this->root = std::make_shared<ZFdNode>(path.root_name().string());
    this->curNode = this->root;
    this->cd(path);
}

ZConsole::~ZConsole()
{
    this->root.reset();
}

#include <locale>

std::wstring _transformString2Wstring(const std::string &s)
{
    setlocale(LC_CTYPE, "en_US.UTF-8");
    const char *_Source = s.c_str();
    size_t len = strlen(_Source) + 1;
    size_t converted = 0;
    wchar_t *wStr = new wchar_t[len];
    mbstowcs_s(&converted, wStr, len, _Source, _TRUNCATE);
    std::wstring result(wStr);
    delete[] wStr;
    return result;
}

std::filesystem::path _getPath(const std::string &p)
{
    std::wstring wdirname = _transformString2Wstring(p);
    std::filesystem::path path(wdirname);
    return path;
}

std::filesystem::path ZConsole::destination(std::filesystem::path &paths)
{
    paths = _getPath(paths.string()).make_preferred();
    std::filesystem::path newPath = fs::absolute(paths);
    // if (paths.is_relative())
    // {
    //     Paths_Ty newPaths = this->curPaths;
    //     for (auto it = paths.begin(); it != paths.end(); it++)
    //     {
    //         if (it->filename().string() == "..")
    //             newPaths.pop_back();
    //         else if (it->filename().string() == ".")
    //             continue;
    //         else
    //             newPaths.push_back(it->filename().string());
    //     }
    //     newPath = zutils::joinPath(newPaths);
    // }
    // else if (paths.is_absolute())
    // {
    //     newPath = paths;
    // }
    // else
    // {
    //     throw std::runtime_error("Not a valid path: " + paths.string());
    // }
// verify
#ifdef ZQ_DEBUG
    std::cout << "[debug][ZConsole::destination] path: ";
    for (auto &path : newPath)
    {
        std::cout << path << " - ";
    }
    std::cout << std::endl;
    std::cout << "[debug][ZConsole::destination] path: " << newPath.string() << std::endl;
#endif
    bool is = std::filesystem::is_directory(_getPath(newPath.string()));
    if (!is)
    {
        throw zutils::error("Directory is not exist: " + newPath.string());
    }
    return _getPath(newPath.string());
}

void ZConsole::cd(std::filesystem::path &path)
{
    std::filesystem::path path_through = this->destination(path);
    fs::current_path(path_through);
#ifdef ZQ_INFO
    std::cout << "[info] " << "Will change to directory: " << path_through.string() << std::endl;
#endif
    this->curPaths.clear();
    auto splited_path = zutils::splitPath(path_through.string());
    for (auto &path : splited_path)
    {
        this->curPaths.push_back(path);
    }
#ifdef ZQ_DEBUG
    std::cout << "[DEBUG][ZConsole::cd] path: ";
    for (auto &path : this->curPaths)
    {
        std::cout << path << " - ";
    }
    std::cout << std::endl;
#endif
    auto cur = this->root;
    for (size_t i = 1; i < this->curPaths.size(); i++)
    {
        cur = cur.get()->locate(this->curPaths[i]);
    }
    this->curNode = cur;
}

void ZConsole::scan()
{
    this->curNode.get()->scan();
}

void ZConsole::info()
{
    std::cout << this->curNode.get()->print() << std::endl;
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
