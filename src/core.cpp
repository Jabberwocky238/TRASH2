#include "core.h"
#include <meojson/include/json.hpp>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <codecvt>
#include <locale>

namespace fs = std::filesystem;

int scan_folder_size_to_file(const fs::path& input_folder, const fs::path& output_file_name) {
    try {
        if (fs::exists(input_folder) && fs::is_directory(input_folder)) {
            json::value output;
            scan_folder_size(input_folder, output);
            if (fs::exists(output_file_name)) {
                fs::remove(output_file_name);
                std::cout << "File " << output_file_name<< " deleted successfully." << std::endl;
            }
            std::ofstream output_file(output_file_name);
            output_file << output.format(4);
            output_file.close();
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

inline void scan_folder_size(const fs::path& input_folder, json::value& output) {
    output = get_folder_info(input_folder);
}
inline const std::wstring path_to_wstring(const fs::path& path) {
    return path.filename().wstring();
};
inline const std::string wstring_to_utf8(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(wstr);
};
inline void encounter(MyStruct& folder_info, double file_size, uint64_t file_count) {
    folder_info._file_count += file_count;
    folder_info._mb += file_size;
}

MyStruct get_folder_info(const fs::path& path) {
    MyStruct folder_info;
    folder_info._name = wstring_to_utf8(path_to_wstring(path));
    folder_info._mb = (double)0;
    folder_info._file_count = (uint64_t)0;
    folder_info.children = std::vector<MyStruct>();

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            encounter(folder_info, (double)entry.file_size() / (1024 * 1024), 1);
        } else if (entry.is_directory()) {
            MyStruct subdir_info = get_folder_info(entry.path());
            encounter(folder_info, (double)subdir_info._mb, subdir_info._file_count);
            // Check if subdir size is greater than or equal to 1MB
            if (subdir_info._mb >= 1.0) {  
                folder_info.children.push_back(std::move(subdir_info));
            }
        }
    }

    return folder_info;
}