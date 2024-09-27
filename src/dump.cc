#include "core.h"

// struct _JsonStruct
// {
//     std::string _name;
//     uintmax_t _size;
//     double _mb;
//     int _count;
//     time_t _last_modified;
//     std::vector<_JsonStruct> children;

//     MEO_JSONIZATION(_name, _size, _mb, _count, _last_modified, children);
// };

// _JsonStruct zq_serialize(const ZFolderInfo *fdinfo)
// {

//     auto obj = _JsonStruct();
//     obj._name = fdinfo->name;
//     obj._size = fdinfo->size;
//     obj._mb = (double)(fdinfo->size / 1024) / 1024.0;
//     obj._count = fdinfo->childrenCount;
//     obj._last_modified = fdinfo->last_modified;
//     obj.children = {};
//     for (auto child : fdinfo->children)
//     {
//         obj.children.push_back(zq_serialize(child));
//     }
//     return obj;
// }

// void zq_serialize(const ZConsole &console, json::value &j)
// {
//     j = zq_serialize(console.cur_info);
// }

// void dump(std::string input_folder, std::string output_file_name)
// {
//     ZConsole console(fs::current_path());
//     console.cd(input_folder);
//     console.scan();
//     json::value j;
//     zq_serialize(console, j);
//     std::ofstream output_file(output_file_name);
//     output_file << j;
//     output_file.close();
// }