#include "include/core.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <codecvt>
#include <locale>

namespace fs = std::filesystem;
using json = nlohmann::json;

constexpr char* JSON_NAME = "a_name";
constexpr char* JSON_SIZE_MB = "b_size_mb";
constexpr char* JSON_FILE_COUNT = "c_file_count";
constexpr char* JSON_CHILDREN = "d_children";


int scan_folder_size(const fs::path& input_folder, json& output) {
    try {
        if (fs::exists(input_folder) && fs::is_directory(input_folder)) {
            output = get_folder_info(input_folder);
            std::cout << "Writing results to 'output.json'" << std::endl;
            return EXIT_SUCCESS;
        } else {
            std::cerr << "The provided path does not exist or is not a directory." << std::endl;
            return EXIT_FAILURE;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

std::string wstring_to_utf8(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(wstr);
}

json get_folder_info(const fs::path& path) {
    json folder_info;
    std::wstring wide_str = path.filename().wstring();
    std::string utf8_str = wstring_to_utf8(wide_str);
    folder_info[JSON_NAME] = utf8_str;
    folder_info[JSON_SIZE_MB] = (double)0;
    folder_info[JSON_FILE_COUNT] = (uint64_t)0;
    folder_info[JSON_CHILDREN] = json::array();

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            folder_info[JSON_SIZE_MB] = (double)folder_info[JSON_SIZE_MB] + (double)fs::file_size(entry) / (1024 * 1024);
            folder_info[JSON_FILE_COUNT] = (uint64_t)folder_info[JSON_FILE_COUNT] + (uint64_t)1;
        } else if (entry.is_directory()) {
            json subdir_info = get_folder_info(entry.path());
            folder_info[JSON_SIZE_MB] = (double)subdir_info[JSON_SIZE_MB] + (double)folder_info[JSON_SIZE_MB];
            folder_info[JSON_FILE_COUNT] = (uint64_t)subdir_info[JSON_FILE_COUNT] + (uint64_t)folder_info[JSON_FILE_COUNT];
            // Check if subdir size is greater than or equal to 1MB
            if (subdir_info[JSON_SIZE_MB] >= 1.0) {  
                folder_info[JSON_CHILDREN].push_back(subdir_info);
            }
        }
    }

    return folder_info;
}