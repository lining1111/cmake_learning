# cmake 练习

    本意:现代工程尤其是c++工程,需要使用cmake来管理,所以这里记录一下cmake的使用,为以后方便工程组织来作基础。
    
    现代社会，互联网带来了大数据，大数据成就了人工只能；同时硬件性能的跨越式提升来为高级语言的编程带来了更广阔的应用空间。
    专门为高级语言服务的管理工具，如VUE的VUE-ite，Java的Maven，Python的pip，Go的Go Modules，Rust的Cargo等等。
    C++因为出现的较早，也出现了很多的工程管理工具，如Make，autoMake，CMake，Bazel等等，
    但是cmake因为其灵活性和跨平台性，成为了现代C++工程管理工具的首选。
    cmake的官方文档:https://cmake.org/cmake/help/latest/
    本工程参考的视频 https://www.bilibili.com/video/BV1rqPYehEsW

## 不积跬步，无以至千里；不积小流，无以成江海

## 章节

    练习章节，是将所有的练习内容，写到my_cmake目录下内的一个一个的.cmake文件中，通过在顶级的CMakeLists.txt中include来引入。

    cmake -G 可以列出所有支持的生成器，前面带*的是默认的生成器。

### 设置一个小目标

**理解c++的具体编译过程，然后在理解cmake是如何组织工程，并按照c++的编译过程来完成工程到具体目标的过程，至关重要。**

    1、  add_executable(target_name source1 source2 ...) # 默认生成的是可执行文件
    2、  add_library(target_name source1 source2 ...) # 默认生成的是静态库
    3、  add_library(target_name STATIC|SHARED|MODULE source1 source2 ...) # 指定生成的是静态库，动态库，模块库
    4、  add_executable(target_name)  # 生成可执行文件
         add_library(target_name) # 生成库文件    # 生成库文件的时候，需要指定库文件类型
        target_sources(target_name source1 source2 ...) # 指定生成可执行文件需要的源文件
    
    尝试下手动编译
    假设有 main.cpp  hello.cpp  hello.h 三个文件
    1、 g++ hello.cpp -c -o hello.o
    2、 g++ main.cpp hello.o -o main # g++ 编译成执行文件的时候，可以使用 -I 指定头文件目录 -L 指定库文件目录 -l 指定库文件
    可以在第1步后，通过ar指令将hello编译成库
        # 静态库
        ar rcs libhello.a hello.o #静态库，可以使用 ar -t 查看库文件中的内容 nm -s libhello.a 查看符号表
        # 动态库
        g++ -fPIC -c hello.cpp -o hello.o # 生成与位置无关的代码
        g++ -shared -o libhello.so hello.o # 动态库，可以使用 ldd 查看动态库依赖的库文件   


    link_libraries() # 链接库文件,将库连接到该句后面出现的所有目标上

    target_link_libraries() # 链接库文件，将库连接到指定的目标上，它有很多种写法
        target_link_libraries(target_name item1 item2 item3 ...) # 链接一个或多个库文件
        target_link_libraries(target_name  LINK_PRIVATE|LINK_PUBLIC|LINK_INTERFACE_LIBRARIES item1 item2 item3 ...  ) # 指定链接库的方式，默认是PRIVATE
            LINK_PRIVATE: 在private后面的库仅被link到你的target中，并且终结掉，第三方不能感知你调了啥库
            LINK_PUBLIC:  在public后面的库会被Link到你的target中，并且里面的符号也会被导出，提供给第三方使用
            LINK_INTERFACE_LIBRARIES: 在interface后面引入的库不会被链接到你的target中，只会导出符号
            可以举个场景来描述下上面三个的不同：
            假设有个程序A，A调用了库B，库B又调用了库C，那么：
            A->B->C
            1、A link B时不管是private还是public都没关系，毕竟A不需要导出符号，也没有人以API方式调用它。
                现在主要问题就是B这个库用private还是public.  C是动态库。
            2、如果B是动态或静态库，C是动态库，这个问题就会有影响。同样，如果B、C同为静态库时也会有问题。
                B用private link C,  此时A link B,但是不知道B->C这层关系，可以正常link B. 
                但是会有运行时问题：
                运行时，A->B->C 时，B找不到C中的函数。
                linux下没有直接依赖关系，所有的B/C的依赖都会转到到A下，可以用LDD命令验证，此时A只依赖于B, 不见C, 当B中的函数调用C中的函数时，因为没有加载C, 所以报找不到符号错误。
                解决方案有两种：
                1、A link B时，B用public link C, 这样A就可以感知到B->C的关系，A在运行时就可以加载C了。
                2、A link B时，同样也写上C. 但是因为private的原因，A是不知道C中的符号这事，只能强制 link C到A才能解决。

                其实private/public 解决的是指示问题，本质上可以使用public 来解决， 可以减少坑。
                会有以上三种链接方式的主要原因是：使用 PRIVATE、PUBLIC 和 INTERFACE 关键字来控制依赖项的传递和范围。
                1、PRIVATE: 默认值，依赖项不会传递给其他目标。
                    例如，如果你有一个库 libA，它依赖于另一个库 libB，并且只在 libA 的源文件中使用了 libB 的功能，
                    那么你可以这样写： target_link_libraries(libA PRIVATE libB) 这样，libA 就可以链接到 libB，
                    但是如果有一个可执行文件 app，它依赖于 libA，那么它不会自动链接到 libB，也不能使用 libB 的功能。
                2、PUBLIC: 依赖项会传递给其他目标，并且依赖项的符号也会被导出。
                    例如，如果你有一个库 libA，它依赖于另一个库 libB，并且在 libA 的源文件和头文件中都使用了 libB 的功能，
                    那么你可以这样写： target_link_libraries(libA PUBLIC libB) 这样，libA 就会链接到 libB，
                    并且如果有一个可执行文件 app，它依赖于 libA，那么它也会自动链接到 libB，并且可以使用 libB 的功能。
                3、INTERFACE: 依赖项会传递给其他目标，但依赖项的符号不会导出。
                    例如，如果你有一个库 libA，它依赖于另一个库 libB，并且只在 libA 的头文件中使用了 libB 的功能，
                    那么你可以这样写： target_link_libraries(libA INTERFACE libB) 这样，libA 就不会链接到 libB，
                    但是如果有一个可执行文件 app，它依赖于 libA，那么它会自动链接到 libB，并且可以使用 libB 的功能。

### CMAKE语法之变量

    变量的作用域可以使用
    block来定义，例如：
    block()
        set(<variable> <value>... [PARENT_SCOPE])
    endblock()

    普通变量
    set(<variable> <value>... [PARENT_SCOPE])
        变量名区分大小写
        PARENT_SCOPE: 将变量设置到父作用域中
        不要使用CMAKE_开头的变量名，因为它们是CMake的内部变量
        所有变量的名都存储成字符串，设置变量为多个值时，相当于"var1;var2;..." 这种用分号分割的
        如果定义的变量中存在很多需要转义的符号，如；、${}等，可以使用[[]]来包裹变量内容，如 set(MY_VAR2 [[hello ${world2}]])
        使用变量时 ${variable}
    缓存变量
    set(<variable> <value>... CACHE <type> <docstring> [FORCE])
        CACHE: 将变量设置为缓存变量，缓存变量可以在CMake GUI中设置
        <type>: 变量的类型，可以是STRING、BOOL、PATH、FILEPATH等
    对于BOOL类型的变量，可以使用option命令来定义，如：
        option(<option_variable> "description" <initial_value>)
    可以在命令行定义缓存变量
    cmake -D<variable>=<value> ..
    环境变量
    set(ENV{<variable>} [<value>])
        set(ENV{PATH} “$ENV{PATH}:/new/path”) #将/new/path临时添加到PATH环境变量中
        set(ENV{LD_LIBRARY_PATH} “$ENV{LD_LIBRARY_PATH}:/new/path”) #将/new/path临时添加到LD_LIBRARY_PATH环境变量中

### CMAKE控制流程 if/elseif/else/endif for_each/endforeach while/endwhile break continue
    这部分还存在
    逻辑运算符 AND/OR/NOT
    比较运算符 EQUAL/STREQUAL/LESS/GREATER/LESS_EQUAL/GREATER_EQUAL/STRLESS/STRGREATER/STRLESS_EQUAL/STRGREATER_EQUAL
    字符串运算符 APPEND/STRAPPEND/TOLOWER/TOUPPER/LENGTH/REGEX/REPLACE/STREQUAL/STRLESS/STRGREATER/STRLESS_EQUAL/STRGREATER_EQUAL
    IN_LIST/IN/ZIP_LISTS

    CMake的预定义变量
    CMAKE_CURRENT_SOURCE_DIR 当前CMakeLists.txt所在的目录
    CMAKE_CURRENT_BINARY_DIR 当前正在处理的二进制目录
    CMAKE_CURRENT_LIST_DIR 当前正在处理的CMakeLists.txt所在的目录
    CMAKE_CURRENT_LIST_FILE 当前正在处理的CMakeLists.txt文件
    CMAKE_CURRENT_LIST_LINE 当前正在处理的CMakeLists.txt文件中的行号
    CMAKE_MODULE_PATH CMake模块的搜索路径
    CMAKE_PREFIX_PATH CMake前缀路径
    CMAKE_INSTALL_PREFIX CMake安装路径
    CMAKE_BUILD_TYPE CMake构建类型
    CMAKE_HOST_SYSTEM_PROCESSOR CMake主机系统处理器
    CMAKE_HOST_SYSTEM CMake主机系统
    CMAKE_BINARY_DIR CMake二进制目录
    CMAKE_CXX_STANDARD CMake C++标准
    CMAKE_CXX_STANDARD_REQUIRED CMake C++标准是否必须
    CMAKE_CXX_EXTENSIONS CMake C++扩展是否启用
    CMAKE_CXX_FLAGS CMake C++编译器标志
    CMAKE_CXX_FLAGS_DEBUG CMake C++编译器调试标志
    CMAKE_CXX_FLAGS_RELEASE CMake C++编译器发布标志
    MSVC/MINGW/XCODE
    
    #if
    if(<condition>)
        <commands>
    elseif(<condition>)
        <commands>
    else()
        <commands>
    endif()

    #foreach
    for_each(<variable> IN LISTS <list> [<range>])
        <commands>
    endforeach()

    #while
    while(<condition>)
        <commands>
    endwhile()

### 函数和宏

    函数和宏的区别是，函数有自己的栈信息，而宏就是自定义替换

    #函数
    function(<name> [<arg1> ...])
        <commands>
    endfunction()

    传参方式：
    命名参数
        function(<name> <arg1> <arg2>)
            message(STATUS "arg1: ${arg1}")
            message(STATUS "arg2: ${arg2}")
        endfunction()
    未命名参数
        function(<name> <arg1> <arg2> <arg3>)
            message(STATUS "arg1: ${arg1}")
            message(STATUS "arg2: ${arg2}")
            message("ARGC: ${ARGC}")
            message("ARGV: ${ARGV}")
            message("ARGN: ${ARGN}")
        endfunction()
    关键字参数,是未命名参数的特殊处理方式 
        cmake_parse_arguments(<prefix> <options> <one_value_keywords> <multi_value_keywords> <args>...)
        cmake_parse_arguments(PARSE_ARGV <N> <prefix> <options> <one_value_keywords> <multi_value_keywords>)
        
        function(<name>)
            set(options MY_OPTION)
            set(oneValueArgs MYLIB_PATH)
            set(multiValueArgs SOURCES INCLUDES)
            cmake_parse_arguments(arg_MY_FUNCTION2 "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
        endfunction()
    

    #宏
    macro(<name> [<arg1> ...])
        <commands>
    endmacro()

### 生成器表达式 list string math file
    
    