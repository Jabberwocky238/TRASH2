#include "interact.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <codecvt>
#include <iomanip> // 包含setw函数的头文

namespace fs = std::filesystem;

// inline const std::string path_to_utf8(const fs::path &path)
// {
//     std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
//     return conv.to_bytes(path.filename().wstring());
// };

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
        if (fs::is_directory(entry.path()))
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

    if (input_line.substr(0, 3) == "cd ")
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
    else
    {
        std::cerr << "Unknown command: " << input_line << std::endl;
    }
    return true;
}