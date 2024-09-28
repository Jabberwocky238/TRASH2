#include "zutils.h"

#include <Windows.h>
#include <sstream>

inline static std::string wchar_to_char(const std::wstring& wstr) {
    int required_size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(required_size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], required_size, nullptr, nullptr);
    return str;
}

namespace zutils
{
    error::error(const std::wstring &msg) : message(msg) {}
    const char *error::what() const noexcept
    {
        return wchar_to_char(message).c_str();
    }
    fatal::fatal(const std::wstring &msg) : message(msg) {}
    const char *fatal::what() const noexcept
    {
        return wchar_to_char(message).c_str();
    }

    bool isChinese(char32_t ch)
    {
        return (ch >= U'\u4E00' && ch <= U'\u9FFF') ||
               (ch >= U'\u3400' && ch <= U'\u4DBF') ||
               (ch >= U'\u2000' && ch <= U'\u2A6D') ||
               (ch >= U'\u2A70' && ch <= U'\u2B73') ||
               (ch >= U'\u2B74' && ch <= U'\u2B81') ||
               (ch >= U'\u2B82' && ch <= U'\u2CEA') ||
               (ch >= U'\u2CEB' && ch <= U'\u2EBE') ||
               (ch >= U'\u3000' && ch <= U'\u3134');
    }
    time_t getLastModified(const std::filesystem::path &path)
    {
        std::filesystem::file_time_type ftime = std::filesystem::last_write_time(path);
        time_t cftime = std::chrono::system_clock::to_time_t(
            std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now()));
        return cftime;
    }
    uintmax_t getFolderSize(const std::filesystem::path &path)
    {
        uintmax_t size = 0;
        for (const auto &entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory())
            {
                size += getFolderSize(entry.path());
            }
            else if (entry.is_regular_file())
            {
                size += entry.file_size();
            }
        }
        return size;
    }
    std::vector<std::string> splitPath(const std::string &path)
    {
        std::vector<std::string> builder;
        std::string part;
        for (auto c : path)
        {
            if (c == '/' || c == '\\')
            {
                builder.push_back(part);
                part = "";
            }
            else
            {
                part += c;
            }
        }
        if (part.size() > 0)
            builder.push_back(part);
        return builder;
    };
    std::filesystem::path joinPath(const std::vector<std::string> &paths)
    {
        std::string builder;
        for (auto path : paths)
        {
            builder += path + "/";
        }
        return builder;
    }
    std::vector<std::string> splitString(const std::string &str, char delimiter)
    {
        std::vector<std::string> tokens;
        std::istringstream strStream(str);
        std::string token;
        while (std::getline(strStream, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    };

}