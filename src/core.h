#pragma once
#define ZQ_DEBUG

#include "zfs/fdnode.h"
#include "zfs/zutils.h"
#include "zconsole.h"

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <meojson/include/json.hpp>

void prompt();
// void zq_serialize(const ZConsole &console, json::value& j);
// void dump(std::string input_folder, std::string output_file_name);