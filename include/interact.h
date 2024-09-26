#include <iostream>
#include <filesystem>

#define MY_INFO(_ARGS_) \
    std::cout << "[info] " << _ARGS_ << std::endl;
#define MY_ERROR(_ARGS_) \
    std::cerr << "[error] " << _ARGS_ << std::endl;


void prompt();
void command_dir(const std::filesystem::path&current_dir);
bool prompt_command(const std::string& input_line, std::filesystem::path& current_dir);