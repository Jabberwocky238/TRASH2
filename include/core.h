#include <filesystem>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

json get_folder_info(const fs::path& path); // ��ȡ�ļ�����Ϣ
int scan_folder_size(const fs::path& input_folder, json& output); // ɨ���ļ��д�С