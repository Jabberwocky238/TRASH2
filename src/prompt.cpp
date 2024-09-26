
#include "prompt.h"
#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

inline std::string parse_command(const std::string &input_line)
{
    return input_line.find(' ') == std::string::npos ? input_line : input_line.substr(0, input_line.find(' '));
}

void prompt_command(const std::string &input_line, ZConsole &console)
{
    std::string command = parse_command(input_line);

    if (command == "cd")
    {
        console.cd(input_line.substr(3));
    }
    else if (command == "ls")
    {
        console.ls();
    }
    else if (command == "scan")
    {
        console.scan();
        std::cout << console.cur_info->info() << std::endl;
    }
    else
    {
        std::cerr << "Unknown command: " << input_line << std::endl;
    }
}

void prompt()
{
    ZConsole console(fs::current_path());
    console.PROMPTING();

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD oldMode;
    GetConsoleMode(hInput, &oldMode);

    DWORD newMode = oldMode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
    SetConsoleMode(hInput, newMode);

    DWORD _cNumRead;
    CHAR _buffer[1024];
    std::string _input_line;

    while (true)
    {
        if (!ReadFile(hInput, _buffer, sizeof(_buffer) - 1, &_cNumRead, NULL))
        {
            std::cerr << "ReadFile failed: " << GetLastError() << std::endl;
            break;
        }

        if (_cNumRead > 0)
        {
            _buffer[_cNumRead] = '\0';

            if (strchr(_buffer, '\r') != NULL)
            {
                if (_input_line == "exit" || _input_line == "quit" || _input_line == "q")
                {
                    std::cout << "Gracefully exit." << std::endl;
                    break;
                }
                prompt_command(_input_line, console);
                console.PROMPTING(true);
                _input_line.clear();
            }
            else if (strchr(_buffer, '\t') != NULL && _input_line.empty())
            {
                console.ls();
                console.PROMPTING(true);
            }
            else if (strchr(_buffer, '\b') != NULL)
            {
                if (!_input_line.empty())
                {
                    _input_line.pop_back();
                    std::cout << "\b \b";
                }
            }
            else
            {
                // const char *fixedContent = "Fixed Content";
                // WriteConsole(hOutput, fixedContent, strlen(fixedContent), &cNumRead, NULL);
                WriteConsole(hOutput, _buffer, _cNumRead, &_cNumRead, NULL);
                _input_line += _buffer;
            }
        }
    }

    SetConsoleMode(hInput, oldMode);
    return;
}

struct _JsonStruct
{
    std::string _name;
    uintmax_t _size;
    double _mb;
    int _count;
    time_t _last_modified;
    std::vector<_JsonStruct> children;

    MEO_JSONIZATION(_name, _size, _mb, _count, _last_modified, children);
};

_JsonStruct zq_serialize(const ZFolderInfo *fdinfo)
{

    auto obj = _JsonStruct();
    obj._name = fdinfo->name;
    obj._size = fdinfo->size;
    obj._mb = (double)(fdinfo->size / 1024) / 1024.0;
    obj._count = fdinfo->childrenCount;
    obj._last_modified = fdinfo->last_modified;
    obj.children = {};
    for (auto child : fdinfo->children)
    {
        obj.children.push_back(zq_serialize(child));
    }
    return obj;
}

void zq_serialize(const ZConsole &console, json::value &j)
{
    j = zq_serialize(console.cur_info);
}

void dump(std::string input_folder, std::string output_file_name)
{
    ZConsole console(fs::current_path());
    console.cd(input_folder);
    console.scan();
    json::value j;
    zq_serialize(console, j);
    std::ofstream output_file(output_file_name);
    output_file << j;
    output_file.close();
}