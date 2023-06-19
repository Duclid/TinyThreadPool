cmake_minimum_required(VERSION 3.23)

project(TinyThreadPool)

# 添加可执行文件
add_executable(MyExecutable src/test_thread_pool.cpp src/thread_pool.cpp)

# 添加头文件搜索路径
target_include_directories(MyExecutable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)