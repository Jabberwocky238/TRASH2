#include <include/core.h>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <codecvt>
#include <locale>

namespace fs = std::filesystem;
using json = nlohmann::json;

int main() {
    std::string input_folder = "E:\\1-School";
    std::cout << "请输入路径: ";
    // std::cin >> input_folder;
    std::cout << "输入路径: " << input_folder << std::endl;
    json output;
    
    if(scan_folder_size(input_folder, output) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    std::ofstream output_file("build/output.json");
    output_file << output.dump(4);
    output_file.close();

    return EXIT_SUCCESS;
}
