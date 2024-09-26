#include "interact.h"

#include <iostream>
#include <filesystem>
#include <string>

#include <Windows.h>

namespace fs = std::filesystem;

inline std::string parse_command(const std::string &input_line)
{
    return input_line.find(' ') == std::string::npos ? input_line : input_line.substr(0, input_line.find(' '));
}

void prompt()
{
    fs::path current_dir = fs::current_path();
    std::cout << "[" << current_dir << "]> ";

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD oldMode;
    GetConsoleMode(hInput, &oldMode);

    // 禁用回显和行输入模式
    DWORD newMode = oldMode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
    SetConsoleMode(hInput, newMode);

    DWORD cNumRead;
    CHAR buffer[1024];
    std::string input_line;

    while (true)
    {
        if (!ReadFile(hInput, buffer, sizeof(buffer) - 1, &cNumRead, NULL))
        {
            std::cerr << "ReadFile failed: " << GetLastError() << std::endl;
            break;
        }
        // 检查是否读取到数据
        if (cNumRead > 0)
        {
            buffer[cNumRead] = '\0';
            std::string command = parse_command(input_line);
            if (strchr(buffer, '\r') != NULL)
            {
                if (input_line == "exit" || input_line == "quit" || input_line == "q")
                {
                    MY_INFO("Gracefully exit.")
                    break;
                }
                std::cout << "\nINPUT: " << input_line << std::endl;
                prompt_command(input_line, current_dir);
                std::cout << std::endl << "[" << current_dir << "]> ";
                input_line.clear();
            }
            else if (strchr(buffer, '\t') != NULL && input_line.empty())
            {   
                command_dir(current_dir);
                std::cout << std::endl << "[" << current_dir << "]> ";

            }
            else if (strchr(buffer, '\b') != NULL)
            {   
                if( !input_line.empty() ) {
                    input_line.pop_back();
                    std::cout << "\b \b";
                }
            }
            else
            {
                // const char *fixedContent = "Fixed Content";
                // WriteConsole(hOutput, fixedContent, strlen(fixedContent), &cNumRead, NULL);
                WriteConsole(hOutput, buffer, cNumRead, &cNumRead, NULL);
                input_line += buffer;
            }
        }
    }

    // 恢复原来的输入模式
    SetConsoleMode(hInput, oldMode);
    return;
}
