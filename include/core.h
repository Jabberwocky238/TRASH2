#include <filesystem>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

json get_folder_info(const fs::path& path); // 获取文件夹信息
int scan_folder_size(const fs::path& input_folder, json& output); // 扫描文件夹大小