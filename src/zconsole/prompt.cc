
#include "core.h"
#include "zconsole.h"

#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <xstring>
#include <vector>
#include <tuple>
#include <variant>

namespace fs = std::filesystem;

using _Cmd_Ty = std::wstring;
using _Cmd_V = std::variant<
    std::wstring,
    std::vector<std::wstring>,
    std::nullopt_t>;

inline static std::vector<std::wstring> parse_args(const std::wstring &input_line)
{
    std::vector<std::wstring> result = { L"" };
    bool has_quote = false;
    for (auto &i : input_line)
    {
        if (i == '\"')
        {
            has_quote = !has_quote;
            continue;
        }

        if (i == ' ' && !has_quote)
        {
            // brand new arg
            result.push_back(L"");
            continue;
        }
        result.back().push_back(i);
    }
    return result;
}

inline static std::tuple<_Cmd_Ty, _Cmd_V> parse_command(const std::wstring &input_line)
{
    auto _first_space = input_line.find(' ');
    if (_first_space == std::wstring::npos)
    {
        return std::make_tuple(input_line, std::nullopt);
    }
    else
    {
        auto command = input_line.substr(0, _first_space);
        auto args = input_line.substr(_first_space + 1);
        auto _split = parse_args(args);
#ifdef ZQ_DEBUG
        std::wcout << "[DEBUG] command: " << command << std::endl;
        std::wcout << "[DEBUG] args: ";
        for (auto &i : _split)
        {
            std::wcout << i << "|";
        }
        std::wcout << std::endl;
#endif
        if (_split.size() == 1)
        {
            return std::make_tuple(command, _split[0]);
        }
        else
        {
            return std::make_tuple(command, _split);
        }
    }
}

void ZConsole::run()
{
    ZConsole console;
    console.PROMPTING();

    SetConsoleOutputCP(CP_UTF8); // 设置控制台输出为UTF-8编码
    HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD oldMode;
    GetConsoleMode(hConsole, &oldMode);

    DWORD newMode = oldMode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
    SetConsoleMode(hConsole, newMode);

    DWORD dwRead;
    INPUT_RECORD irIn[1024]; // 可以一次读取多个事件
    std::wstring _input_line;

    bool not_enter = false;

    try
    {
        while (true)
        {
            try
            {
                not_enter = true;
                if (ReadConsoleInputW(hConsole, irIn, 1024, &dwRead))
                {
                    // std::cout << "[ReadConsoleInput success: " << dwRead << ']' << std::endl;
                    // 判断特殊值：回车，TAB, 上下箭头，退格键
                    if (irIn[0].EventType == KEY_EVENT && irIn[0].Event.KeyEvent.bKeyDown)
                    {
                        switch (irIn[0].Event.KeyEvent.wVirtualKeyCode)
                        {
                        case VK_UP:
                            std::cout << "Up arrow key pressed" << std::endl;
                            break;
                        case VK_DOWN:
                            std::cout << "Down arrow key pressed" << std::endl;
                            break;
                        case VK_TAB:
                            std::cout << std::endl;
                            std::cout << "Tab key pressed" << std::endl;
                            if (!_input_line.empty())
                            {
                                _input_line.clear();
                            }
                            console.ls();
                            console.PROMPTING(true);
                            break;
                        case VK_RETURN:
                            not_enter = false;
                            std::cout << std::endl;
                            // std::cout << "Enter key pressed" << std::endl;
                            if (!_input_line.empty())
                            {
                                auto [command, value] = parse_command(_input_line);
                                if (command == L"cd")
                                {
                                    if (std::holds_alternative<std::wstring>(value))
                                    {
                                        std::filesystem::path path_to = std::get<std::wstring>(value);
                                        console.cd(path_to);
                                    }
                                    else
                                        throw zutils::error(L"Invalid argument: " + _input_line);
                                }
                                else if (command == L"ls")
                                {
                                    if (std::holds_alternative<std::nullopt_t>(value))
                                    {
                                        console.ls();
                                    }
                                    else
                                        throw zutils::error(L"Not support ls with argument");
                                }
                                else if (command == L"scan")
                                {
                                    if (std::holds_alternative<std::nullopt_t>(value))
                                    {
                                        console.scan();
                                        console.info();
                                    }
                                    else
                                        throw zutils::error(L"Command 'scan' should not have argument");
                                }
                                else
                                    throw zutils::error(L"Unknown command: " + command);
                                _input_line.clear();
                            }
                            console.PROMPTING(true);
                            break;
                        case VK_BACK:
                            // std::cout << "Backspace key pressed" << std::endl;
                            if (!_input_line.empty())
                            {
                                auto last = _input_line.back();
                                if(zutils::isChinese(last)) 
                                {
                                    std::cout << "\b\b  \b\b";
                                    _input_line.pop_back();
                                }
                                else
                                {
                                    std::cout << "\b \b";
                                    _input_line.pop_back();
                                }
                            }
                            break;
                        }
                    }

                    if (not_enter)
                    {
                        for (DWORD i = 0; i < dwRead; i++)
                        {
                            if (irIn[i].EventType == KEY_EVENT &&
                                irIn[i].Event.KeyEvent.bKeyDown &&
                                irIn[i].Event.KeyEvent.wVirtualKeyCode != VK_RETURN &&
                                irIn[i].Event.KeyEvent.wVirtualKeyCode != VK_BACK &&
                                irIn[i].Event.KeyEvent.uChar.UnicodeChar != 0)
                            {
                                auto __char = irIn[i].Event.KeyEvent.uChar.UnicodeChar;
                                _input_line += __char;
                                std::wcout << __char;
                            }
                        }
                        // std::cout << dwRead << ": " << _input_line << std::endl;
                    }
                }
            }
            catch (const zutils::error &e)
            {
                std::cerr << e.what() << std::endl;
                _input_line.clear();
                console.PROMPTING(true);
            }
        }
    }
    catch (const zutils::fatal &e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << GetLastError() << std::endl;
    }
    SetConsoleMode(hConsole, oldMode); // 恢复控制台模式
    return;
}

// void prompt()
// {
//     ZConsole console;
//     console.PROMPTING();

//     HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
//     HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
//     DWORD oldMode;
//     GetConsoleMode(hInput, &oldMode);

//     DWORD newMode = oldMode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
//     SetConsoleMode(hInput, newMode);

//     DWORD _cNumRead;
//     CHAR _buffer[1024];
//     std::string _input_line;

//     while (true)
//     {
//         if (!ReadFile(hInput, _buffer, sizeof(_buffer) - 1, &_cNumRead, NULL))
//         {
//             std::cerr << "ReadFile failed: " << GetLastError() << std::endl;
//             break;
//         }

//         if (_cNumRead > 0)
//         {
//             _buffer[_cNumRead] = '\0';

//             if (strchr(_buffer, '\r') != NULL)
//             {
//                 if (_input_line == "exit" || _input_line == "quit" || _input_line == "q")
//                 {
//                     std::cout << "Gracefully exit." << std::endl;
//                     break;
//                 }
//                 prompt_command(_input_line, console);
//                 console.PROMPTING(true);
//                 _input_line.clear();
//             }
//             else if (strchr(_buffer, '\t') != NULL && _input_line.empty())
//             {
//                 console.ls();
//                 console.PROMPTING(true);
//                 _input_line.clear();
//             }
//             else if (strchr(_buffer, '\b') != NULL)
//             {
//                 if (!_input_line.empty())
//                 {
//                     _input_line.pop_back();
//                     std::cout << "\b \b";
//                 }
//             }
//             else
//             {
//                 WriteConsole(hOutput, _buffer, _cNumRead, &_cNumRead, NULL);
//                 _input_line += _buffer;
//             }
//         }
//     }

//     SetConsoleMode(hInput, oldMode);
//     return;
// }
