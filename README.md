- [中文](README.md)
- [EN](README_EN.md)

# 扫盘算文件夹大小
玩具项目

# 使用方法
```bash
demo -i ./ -o ./output.json
demo # 直接命令行交互
```

# 构建
```bash
setup clean
setup build
setup run  # 命令行模式
```

# 鸣谢
- [meojson](https://github.com/MistEO/meojson)
- [cxxopts](https://github.com/jarro2783/cxxopts)

```sh
git submodule add -f https://github.com/MistEO/meojson.git thirdparty/meojson
git submodule add -f https://github.com/jarro2783/cxxopts.git thirdparty/cxxopts
git submodule add -f https://github.com/doctest/doctest.git thirdparty/doctest
```