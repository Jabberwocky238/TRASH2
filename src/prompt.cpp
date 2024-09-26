
#include "prompt.h"
#include "fswalk.h"

#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <string>


namespace fs = std::filesystem;

inline std::string parse_command(const std::string &input_line)
{
    return input_line.find(' ') == std::string::npos ? input_line : input_line.substr(0, input_line.find(' '));
}

void MY_PROMPT(const fs::path& prompt, bool enter = false) {
    if (enter) {
        std::cout << std::endl;
    }
    std::cout << "[" << prompt << "]> ";
}
    
void prompt()
{
    Console console(fs::current_path());
    MY_PROMPT(console.cur_path);

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
            std::string command = parse_command(_input_line);
            
            if (strchr(_buffer, '\r') != NULL)
            {
                if (_input_line == "exit" || _input_line == "quit" || _input_line == "q")
                {
                    std::cout << "Gracefully exit." << std::endl;
                    break;
                }
                std::cout << "\nINPUT: " << _input_line << std::endl;
                prompt_command(_input_line, console);
                MY_PROMPT(console.cur_path, true);
                _input_line.clear();
            }
            else if (strchr(_buffer, '\t') != NULL && _input_line.empty())
            {   
                command_dir(console.cur_path);
                MY_PROMPT(console.cur_path, true);
            }
            else if (strchr(_buffer, '\b') != NULL)
            {   
                if( !_input_line.empty() ) {
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
