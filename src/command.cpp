#include "command.h"
#include "fswalk.h"

#include <iostream>
#include <filesystem>
#include <string>
#include <codecvt>
#include <iomanip>

namespace fs = std::filesystem;

Console::Console(const std::filesystem::path &path)
{
    this->cur_path = path;
    // this->cur_info = FolderInfo::init(path);
    this->cur_info = nullptr;
    this->cd(path);
}

Console::~Console()
{
    delete this->cur_info->root();
}

bool Console::cd(const std::filesystem::path &path)
{
    try
    {
        fs::current_path(path);
        this->cur_path = fs::current_path();
        // this->cur_path.string();
        std::vector<std::string> path_through = zq_fswalk::split_path(this->cur_path.string());
        for (auto &name : path_through)
        {
            std::cout << "[debug] " << name << std::endl;
        }
        // std::vector<std::string> path_through = {};
        std::cout << "[info] " << "Change to directory: " << this->cur_path << std::endl;

        if (this->cur_info != nullptr)
        {
            // not null and found
            // std::cout << "[info] " << "cur_info is not nullptr" << std::endl;
            if (this->cur_info->root()->find_tree(path_through, 1) != nullptr)
            {
                this->cur_info = this->cur_info->root()->find_tree(path_through, 1);
                std::cout << "[info] " << "founded" << std::endl;
                return true;
            }
        }
        std::cout << "[info] " << "cur_info is nullptr, Need to build new folder info" << std::endl;

        FolderInfo *root = this->cur_info != nullptr ? this->cur_info->root() : new FolderInfo(path_through[0]);
        std::string _path = path_through[0];
        path_through.erase(path_through.begin());

        for (auto &name : path_through)
        {
            auto *child = root->find_children(name);
            // std::cout << "[info] " << "build: " << info << std::endl;
            if (child == nullptr)
            {
                // std::cout << "[info] " << "create" << std::endl;
                child = new FolderInfo(name);
                child->parent = root;
                root->children.push_back(child);
            }
            root = child;
            // std::cout << "[info] " << "root update: " << root << std::endl;
        }
        this->cur_info = root;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return false;
}

FolderInfo *Console::scan()
{
    return nullptr;
}

inline std::string parse_command(const std::string &input_line)
{
    return input_line.find(' ') == std::string::npos ? input_line : input_line.substr(0, input_line.find(' '));
}

void command_dir(const fs::path &current_dir)
{
    size_t _cnt = 0;
    std::cout << std::endl;
    for (const auto &entry : fs::directory_iterator(current_dir))
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

bool prompt_command(const std::string &input_line, Console &console)
{
    std::string command = parse_command(input_line);

    if (command == "cd")
    {
        if (fs::is_directory(input_line.substr(3)))
        {
            return console.cd(input_line.substr(3));
        }
        else
        {
            std::cerr << "No such directory: " << input_line.substr(3) << std::endl;
        }
    }
    else if (command == "ls")
    {
        command_dir(console.cur_path);
    }
    else if (command == "scan")
    {
        FolderInfo *info = console.scan();
        std::cout << info->info() << std::endl;
        delete info;
    }
    else
    {
        std::cerr << "Unknown command: " << input_line << std::endl;
    }
    return true;
}