cmake_minimum_required(VERSION 3.30)
project(cmake_learning)

set(CMAKE_CXX_STANDARD 14)

# 函数和宏
include(my_cmake/my_function.cmake)
include(my_cmake/my_macro.cmake)

#自定义变量
message("练习-自定义变量")
set(MY_VAR "hello world")
message(STATUS "MY_VAR: ${MY_VAR}")

set(MY_VAR2 [[hello ${world2}]])
message(STATUS "MY_VAR2: ${MY_VAR2}")

#函数和宏
message("练习-函数和宏")
my_function(hello world)
my_function1(hello world 1 2 3)
my_function2(hello
            MYLIB_PATH "/usr/local/lib"
            SOURCES "main.cpp"
            INCLUDES "include" "include2")

my_function2(hello
        MYLIB_PATH "/usr/local/lib" "/usr/local/lib2"
        SOURCES
        INCLUDES "include" "include2")
my_macro(World)

#list
message("练习-list")
set(listVar a b c)
list(LENGTH listVar len)
message(STATUS "list length: ${len}")
list(GET listVar 0 first)
message(STATUS "list first: ${first}")

add_executable(cmake_learning main.cpp)
