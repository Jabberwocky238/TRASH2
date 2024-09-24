#include "core.h"
#include <cxxopts/include/cxxopts.hpp>

#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;


int main(int argc, char* argv[]) {
    cxxopts::Options options("MyProgram", "One line description of MyProgram");
    options.add_options()
        ("m,mode", "launch mode", cxxopts::value<std::string>()->default_value("interact"))
        ("i,input", "input dir path", cxxopts::value<std::string>()->default_value("E:\\1-School"))
        ("o,output", "output file path", cxxopts::value<std::string>()->default_value("output.json"))
    ;

    auto result = options.parse(argc, argv);
    std::string mode = result["mode"].as<std::string>();
    if (mode == std::string("interact")) {
        fs::path current_dir = fs::current_path();
        std::string input_line;
        while (true) {
            std::cout << "[" << current_dir << "]> ";
            std::getline(std::cin, input_line);

            if (input_line == "exit") {
                std::cout << "Gracefully exit." << std::endl;
                break;
            } else if (input_line.substr(0, 3) == "cd ") {
                if(fs::is_directory(input_line.substr(3))) {
                    std::cout << "Change to directory: " << input_line.substr(3) << std::endl;
                    fs::current_path(input_line.substr(3));
                    current_dir = fs::current_path();
                } else {
                    std::cerr << "No such directory: " << input_line.substr(3) << std::endl;
                }
            } else {
                std::cerr << "Unknown command: " << input_line << std::endl;
            }
        }
    } else {
        std::string input_folder = result["input"].as<std::string>();
        std::string output_file_name = result["output"].as<std::string>();
        std::cout << "dir path: " << input_folder << std::endl;
        
        scan_folder_size_to_file(input_folder, output_file_name);
    }
    return EXIT_SUCCESS;
}
