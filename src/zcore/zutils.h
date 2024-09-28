#pragma once
#include "core.h"

#include <filesystem>
#include <string>
#include <vector>
#include <chrono>
#include <exception>

namespace zutils
{
    bool isChinese(char32_t ch);
    time_t getLastModified(const std::filesystem::path &path);
    uintmax_t getFolderSize(const std::filesystem::path &path);
    std::vector<std::string> splitPath(const std::string &path);
    std::filesystem::path joinPath(const std::vector<std::string> &paths);
    std::vector<std::string> splitString(const std::string &str, char delimiter);

    class error : public std::exception
    {
    private:
        std::wstring message;

    public:
        error(const std::wstring &msg);
        virtual const char *what() const noexcept;
    };

    class fatal : public std::exception
    {
    private:
        std::wstring message;

    public:
        fatal(const std::wstring &msg);
        virtual const char *what() const noexcept;
    };
}
