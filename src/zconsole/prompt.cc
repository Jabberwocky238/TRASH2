
#include "core.h"
#include "zconsole.h"

#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

inline static std::string parse_command(const std::string &input_line)
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
        console.info();
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
                _input_line.clear();
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
                WriteConsole(hOutput, _buffer, _cNumRead, &_cNumRead, NULL);
                _input_line += _buffer;
            }
        }
    }

    SetConsoleMode(hInput, oldMode);
    return;
}
