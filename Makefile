# 定义编译器
CXX := g++
# 定义编译选项，启用C++14标准
CXXFLAGS := -std=c++14 -Wall -Wextra -pedantic

# 定义源文件和目标文件
EXECUTABLE := MyExecutable

all: $(EXECUTABLE)
	@echo "[Build successful!]"
	@./build/src/Debug/$(EXECUTABLE)

# 构建可执行文件
$(EXECUTABLE): 
	@cmake -Bbuild -H. 
	@cmake --build build --target $(EXECUTABLE)

# 清理目标，删除生成的文件
clean:
	@rm -rf build/.cmake/*
	@rm -rf build/*

# 运行目标，构建并运行程序
run: $(EXECUTABLE)
	@$(EXECUTABLE)

# 伪目标声明，防止Makefile中定义的规则与文件名冲突
.PHONY: all clean run
