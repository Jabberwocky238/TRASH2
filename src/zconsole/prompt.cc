
#include "core.h"
#include "zconsole.h"

#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <tuple>
#include <variant>
#include <codecvt>
#include <system_error>
#include <charconv>

namespace fs = std::filesystem;

using _Cmd_Ty = std::string;
using _Cmd_V = std::variant<
    std::string,
    std::vector<std::string>,
    std::nullopt_t>;

inline static std::vector<std::string> parse_args(const std::string &input_line)
{
    std::vector<std::string> result = {""};
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
            result.push_back("");
            continue;
        }
        result.back().push_back(i);
    }
    return result;
}

inline static std::tuple<_Cmd_Ty, _Cmd_V> parse_command(const std::string &input_line)
{
    auto _first_space = input_line.find(' ');
    if (_first_space == std::string::npos)
    {
        return std::make_tuple(input_line, std::nullopt);
    }
    else
    {
        std::string command = input_line.substr(0, _first_space);
        std::string args = input_line.substr(_first_space + 1);
        std::vector<std::string> _split = parse_args(args);
#ifdef ZQ_DEBUG
        std::cout << "[DEBUG] command: " << command << std::endl;
        std::cout << "[DEBUG] args: ";
        for (auto &i : _split)
        {
            std::cout << i << "|";
        }
        std::cout << std::endl;
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
    std::string _input_line;

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
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
                                if (command == "cd")
                                {
                                    if (std::holds_alternative<std::string>(value))
                                    {
                                        std::filesystem::path path_to = std::get<std::string>(value);
                                        console.cd(path_to);
                                    }
                                    else
                                        throw zutils::error("Invalid argument: " +  _input_line);
                                }
                                else if (command == "ls")
                                {
                                    if (std::holds_alternative<std::nullopt_t>(value))
                                    {
                                        console.ls();
                                    }
                                    else
                                        throw zutils::error("Not support ls with argument");
                                }
                                else if (command == "scan")
                                {
                                    if (std::holds_alternative<std::nullopt_t>(value))
                                    {
                                        console.scan();
                                        console.info();
                                    }
                                    else
                                        throw zutils::error("Command 'scan' should not have argument");
                                }
                                else
                                    throw zutils::error("Unknown command: " + command);
                                _input_line.clear();
                            }
                            console.PROMPTING(true);
                            break;
                        case VK_BACK:
                            // std::cout << "Backspace key pressed" << std::endl;
                            if (!_input_line.empty())
                            {
                                auto last = _input_line.back();
                                wchar_t unicode_char;
                                std::mbtowc(&unicode_char, &last, sizeof(last));
                                if(zutils::isChinese(unicode_char)) 
                                {
                                    std::cout << "\b\b  \b\b";
                                    _input_line.pop_back();
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
                                std::string __char = converter.to_bytes(irIn[i].Event.KeyEvent.uChar.UnicodeChar);
                                _input_line += __char;
                                std::cout << __char;
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
