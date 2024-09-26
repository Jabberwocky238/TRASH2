#include "interact.h"
#include "fswalk.h"

#include <iostream>
#include <filesystem>
#include <string>
#include <codecvt>
#include <iomanip> 

namespace fs = std::filesystem;

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

bool prompt_command(const std::string &input_line, fs::path &current_dir)
{
    std::string command = parse_command(input_line);

    if (command == "cd")
    {
        if (fs::is_directory(input_line.substr(3)))
        {
            MY_INFO("Change to directory: " << input_line.substr(3));
            fs::current_path(input_line.substr(3));
            current_dir = fs::current_path();
        }
        else
        {
            std::cerr << "No such directory: " << input_line.substr(3) << std::endl;
        }
    }
    else if (command == "ls"){
        command_dir(current_dir);
    }
    else if (command == "scan"){
        FolderInfo* info = scan_folder(current_dir);
        std::cout << info->info() << std::endl;
        delete info;
    }
    else
    {
        std::cerr << "Unknown command: " << input_line << std::endl;
    }
    return true;
}