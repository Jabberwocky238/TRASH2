#include "core.h"
#include "zfs/fdnode.h"
#include "zfs/zutils.h"
#include "zconsole.h"

#include <cxxopts/include/cxxopts.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    cxxopts::Options options("MyProgram", "One line description of MyProgram");
    std::string default_output_file_name = "output.json";
#ifdef ZQ_DEBUG
    default_output_file_name = "bin/output.json";
#endif
    options.add_options()
        ("i,input", "input dir path", cxxopts::value<std::string>()->default_value(""))
        ("o,output", "output file path", cxxopts::value<std::string>()->default_value(default_output_file_name))
    ;

    auto result = options.parse(argc, argv);
    std::string input_folder = result["input"].as<std::string>();
    std::string output_file_name = result["output"].as<std::string>();
    
    if (input_folder == "") {
        prompt();
    } else {
        // dump(input_folder, output_file_name);
    }
    return EXIT_SUCCESS;
}
