#include <filesystem>
#include <meojson/include/json.hpp>

namespace fs = std::filesystem;

struct MyStruct
{
    std::string _name;
    double _mb;
    uint64_t _file_count;
    std::vector<MyStruct> children;

    MEO_JSONIZATION(_name, _mb, _file_count, children);
};

MyStruct get_folder_info(const fs::path& path);
void scan_folder_size(const fs::path& input_folder, json::value& output);
int scan_folder_size_to_file(const fs::path& input_folder, const fs::path& output_file_name);
