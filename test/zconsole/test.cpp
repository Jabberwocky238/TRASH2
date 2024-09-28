#include "core.h"
#include "zconsole.h"

#include <string>
#include <iostream>
#include <filesystem>
#include <cassert>

namespace fs = std::filesystem;
using namespace std;

int main() {
    ZConsole console;
    fs::path to1 = string("../../test/zconsole/这是中文路径");
    console.cd(to1);
    console.scan();
    assert(console.curNode.get()->path() == to1);

    fs::path to2 = string("..");
    console.cd(to2);
    console.ls();
    assert(console.curNode.get()->path() == to2);

    fs::path to3 = string("/这是中文路径/这是中文子文件夹2222abc你好");
    console.cd(to3);
    console.scan();
    assert(console.curNode.get()->path() == to3);
}