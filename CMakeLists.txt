cmake_minimum_required(VERSION 3.30)
project(cmake_learning)

set(CMAKE_CXX_STANDARD 17)

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
list(APPEND listVar d)
message(STATUS "list append: ${listVar}")
list(REMOVE_ITEM listVar a)
message(STATUS "list remove item: ${listVar}")
list(REMOVE_AT listVar 0)
message(STATUS "list remove at: ${listVar}")
list(JOIN listVar " " joinStr)
message(STATUS "list join: ${joinStr}")
#string
message("练习-string")
string(STRIP "  hello world  " str)
message(STATUS "string strip: ${str}")
string(FIND interesting in fIndex)
message(STATUS "string find: ${fIndex}")
string(FIND interesting in rIndex REVERSE)
message(STATUS "string find reverse: ${rIndex}")
#math
message("练习-math")
math(EXPR a "1+2*6/2")
message(STATUS "math a: ${a}")
math(EXPR b "${a}*3")
message(STATUS "math b: ${b}")
#file
message("练习-file")
message("file 获取目录中的源文件列表")
file(GLOB varFilelist LIST_DIRECTORIES false CONFIGURE_DEPENDS RELATIVE ${CMAKE_SOURCE_DIR} "*.cpp")
message("varFilelist: ${varFilelist}")
#生成器表达式
message("练习-生成器表达式")
message($<1:hello>)
message($<CONFIG>)
#会在构建目标生成gen.txt文件,内容为platform = Linux.即PLATFORM_ID为Linux
file(GENERATE OUTPUT ${CMAKE_BINARY_DIR}/gen.txt CONTENT "platform = $<PLATFORM_ID>" TARGET myAPP)


add_executable(cmake_learning main.cpp)

add_executable(my_unique_ptr_test my_unique_ptr/my_unique_ptr_test.cpp)
add_executable(my_shared_ptr_test my_shared_ptr/my_shared_ptr_test.cpp)
add_executable(my_weak_ptr_test my_weak_ptr/my_weak_ptr_test.cpp)

add_executable(my_lambda_test my_lambda/my_lambda_test.cpp)