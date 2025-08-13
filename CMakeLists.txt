cmake_minimum_required(VERSION 3.10)
project(TextTransformer)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含头文件目录
include_directories(include)

# 添加主程序
add_executable(transform 
    src/main.cpp
    src/core/transform.cpp
    src/utils/io_utils.cpp
)

# 添加测试程序
add_executable(test_transform
    test/test_transform.cpp
    src/core/transform.cpp
    src/utils/io_utils.cpp
)

# Windows下添加控制台选项
if(WIN32)
    target_link_options(transform PRIVATE -mconsole)
    target_link_options(test_transform PRIVATE -mconsole)
endif()