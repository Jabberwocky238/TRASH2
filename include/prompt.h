#pragma once
#include "fswalk.h"
#include "command.h"

#include <iostream>
#include <filesystem>


void prompt();
void command_dir(const std::filesystem::path&current_dir);
void prompt_command(const std::string &input_line, Console& console);