#include "core.h"
#include "interact.h"
#include <cxxopts/include/cxxopts.hpp>

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;


int main(int argc, char* argv[]) {
    cxxopts::Options options("MyProgram", "One line description of MyProgram");
    options.add_options()
        ("i,input", "input dir path", cxxopts::value<std::string>()->default_value(""))
        ("o,output", "output file path", cxxopts::value<std::string>()->default_value("output.json"))
    ;

    auto result = options.parse(argc, argv);
    std::string input_folder = result["input"].as<std::string>();
    std::string output_file_name = result["output"].as<std::string>();

    if (input_folder == "") {
        prompt();
    } else {
        std::cout << "dir path: " << input_folder << std::endl;
        
        scan_folder_size_to_file(input_folder, output_file_name);
    }
    return EXIT_SUCCESS;
}
