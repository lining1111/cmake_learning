# cmake/c++ 练习

    本意:现代工程尤其是c++工程,需要使用cmake来管理,所以这里记录一下cmake的使用,为以后方便工程组织来作基础。
    
    现代社会，互联网带来了大数据，大数据成就了人工只能；同时硬件性能的跨越式提升来为高级语言的编程带来了更广阔的应用空间。
    专门为高级语言服务的管理工具，如VUE的VUE-ite，Java的Maven，Python的pip，Go的Go Modules，Rust的Cargo等等。
    C++因为出现的较早，也出现了很多的工程管理工具，如Make，autoMake，CMake，Bazel等等，
    但是cmake因为其灵活性和跨平台性，成为了现代C++工程管理工具的首选。
    cmake的中文文档:https://cmake.com.cn/
    视频地址 https://www.bilibili.com/video/BV1rqPYehEsW

    下面的内容会以cmake学习为主，并辅以现代c++编程技巧展开。现代c++编程技巧，包括：智能指针、Lambda表达式、模板编程、协程编程、多线程编程等等。

## 不积跬步，无以至千里；不积小流，无以成江海

    人工智能，会再一次的将ROS2机器人开发，以及深度学习提高热度，嵌入式编程可能不再是设备上简单的读取、写入、执行，
    而是可以在一个更大的系统上，充分发挥单一模组的服务特质，并将系统以更完备的姿态展示给世界的一个基石编程。
    学习cmake与c++的心态应该摒弃更高级的语言不是功能更强大、更方便吗的心态。练习尺有所长、寸有所短的心态。

## CMake章节

    练习章节，是将所有的练习内容，写到my_cmake目录下内的一个一个的.cmake文件中，通过在顶级的CMakeLists.txt中include来引入。

    cmake -G 可以列出所有支持的生成器，前面带*的是默认的生成器。

    **理解c++的具体编译过程，然后在理解cmake是如何组织工程，并按照c++的编译过程来完成工程到具体目标的过程，至关重要。**
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

### 设置一个小目标

    https://cmake.com.cn/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html
    https://cmake.com.cn/cmake/help/latest/guide/tutorial/Adding%20a%20Library.html

    1、  add_executable(target_name source1 source2 ...) # 默认生成的是可执行文件
    2、  add_library(target_name source1 source2 ...) # 默认生成的是静态库
    3、  add_library(target_name STATIC|SHARED|MODULE source1 source2 ...) # 指定生成的是静态库，动态库，模块库
    4、  add_executable(target_name)  # 生成可执行文件
         add_library(target_name) # 生成库文件    # 生成库文件的时候，需要指定库文件类型
        target_sources(target_name source1 source2 ...) # 指定生成可执行文件需要的源文件


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

    https://cmake.com.cn/cmake/help/latest/manual/cmake-variables.7.html
    
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

    https://cmake.com.cn/cmake/help/latest/manual/cmake-commands.7.html
    https://cmake.com.cn/cmake/help/latest/manual/cmake-env-variables.7.html
    https://cmake.com.cn/cmake/help/latest/manual/cmake-variables.7.html
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

    https://cmake.com.cn/cmake/help/latest/command/function.html
    https://cmake.com.cn/cmake/help/latest/command/macro.html
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

### list string math file 生成器表达式

    https://cmake.com.cn/cmake/help/latest/command/list.html
    https://cmake.com.cn/cmake/help/latest/command/string.html
    https://cmake.com.cn/cmake/help/latest/command/math.html
    https://cmake.com.cn/cmake/help/latest/command/file.html
    https://cmake.com.cn/cmake/help/latest/manual/cmake-generator-expressions.7.html

    list()
        list(LENGTH <list> <out-var>) #LENGTH/GET/REMOVE/APPEND/INSERT/REVERSE/SORT/JOIN/SUBLIST/TRANSFORM
    string()
        string(STRIP <string> <out-var>) #STRIP/REPLACE/FIND/REPLACE/TOLOWER/TOUPPER/LENGTH/COMPARE/GENEX_STRIP/GENEX_EVAL/GENEX_STRIP/GENEX_EVAL
    math()
        math(EXPR <variable> <expression>) #EXPR/ABS/ABSOLUTE/CEIL/CONSTANT/FACTORIAL/FLOOR/LOG10/LOG2/POW/ROUND/SQRT/SIN/COS/TAN/ASIN/ACOS/ATAN
    file()
        file(<cmd> <args>...) #GLOB/READ/REMOVE/RENAME/RECURSE/TO_CMAKE_PATH/TO_NATIVE_PATH/MAKE_DIRECTORY/REMOVE_DIRECTORY/REMOVE/RENAME/RENAME
    常见的生成器表达式：
        $<CONFIGURATION>：配置名称，如Debug、Release等。
        $<PLATFORM_ID>：平台名称，如Windows、Linux等。
        $<COMPILE_LANGUAGE>：编译语言，如C、C++等。
        $<C_COMPILER_ID>：C编译器ID，如GNU、Clang等。
        $<CXX_COMPILER_ID>：C++编译器ID，如GNU、Clang等。
        $<CXX_STANDARD>：C++标准，如C++11、C++14等。
        $<CXX_COMPILER_VERSION>：C++编译器版本，如4.8、5.3等。
        $<TARGET_NAME:...>：目标名称，如myApp。
        $<TARGET_FILE:...>：目标文件路径，如myApp.exe。

### CMake目录分层设计

    将模块通过文件夹以及文件夹集合成库的方式，对工程的各个模组作解耦操作，是CMake目录分层设计的主要目的。

### 配置和使用三方库

    CMAKE_MODULE_PATH：指定CMake模块的搜索路径，默认为/usr/share/cmake-<version>/Modules/。
        find_package(<package> [QUIET] [REQUIRED] [COMPONENTS <components>])：查找三方库，如find_package(OpenCV REQUIRED)。
        这条命令就是在CMake的模块搜索路径中，找到Find<package>.cmake文件，然后执行该文件中的内容，查找三方库并设置相关的变量。
        如 Imported Targets：OpenCV::core、OpenCV::imgproc等。
#### CMake生成一个自定义库并安装

    1、生成库
        add_library(<name> [STATIC | SHARED | MODULE] [EXCLUDE_FROM_ALL] source1 [source2 ...])
    2、生成配置文件
        include(CMakePackageConfigHelpers)
        write_basic_package_version_file(<name> VERSION <version> [COMPATIBILITY AnyNewerVersion] [PREFIX <prefix>])
        configure_package_config_file(<template> <output> INSTALL_DESTINATION <dir> [PATH_VARS <var1> ...] [QUIET])
    3、导出目标
        install(TARGETS <name> [EXPORT <export_name>])
    4、导出配置
        install(EXPORT <export_name> [FILE <filename>] [NAMESPACE <names>] [DESTINATION <dir>])
    5、安装复制文件
        install(FILES <file> ... [DESTINATION <dir>])
        

#### CMake可以与gtest一起使用

    在工程主目录的CMakeLists.txt文件中添加：
        # 测试
        enable_testing()    
        add_subdirectory(test EXCLUDE_FROM_ALL)
    
    在测试文件夹中的CMakeLists.txt文件中添加：
        # 自动下载依赖
        include(FetchContent)
        # 获取项目。可以是一个URL也可以是一个Git仓库。
        FetchContent_Declare(gtest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.10.0
        )
        # 获取我们需要库,然后引入项目。
        FetchContent_MakeAvailable(gtest)

        # 添加可执行文件
        add_executable(myTest main.cpp)
        # 链接库
        target_link_libraries(myTest PRIVATE gtest gtest_main)
        # 包含gtest模块
        include(GoogleTest) 
        # 发现测试用例
        gtest_discover_tests(myTest)


    CMAKE_INSTALL_INCLUDEDIR：指定头文件的安装路径，默认为/usr/local/include/。

## Modern C++章节

    现代c++的中文文档 https://changkun.de/modern-cpp/ 
    这个作者有这个的代码仓库 https://github.com/changkun/modern-cpp-tutorial
    视频地址 https://www.bilibili.com/video/BV1w2ppenEFj    

### const & constexpr
    const:常量，修饰变量，函数，类，对象，指针，引用
    constexpr:常量表达式，修饰变量，函数，类，对象，指针，引用。能用于复杂的对象构造或函数调用。
    const和constexpr的区别：
        const：常量值在编译时确定，但并不保证在编译时求值。它可以在运行时被修改。
        constexpr：常量表达式在编译时求值，并且其值必须在编译时确定。这意味着constexpr变量或函数的返回值必须是编译时常量。
    应用场景：
        const：通常用于需要常量值但不要求编译时求值的场景。
        constexpr：用于需要编译时求值的场景，例如数组大小、模板参数等。

    总结来说，constexpr是const的增强版，是const的子集。在需要优化性能和确保代码在编译时就能确定值的场景中非常有用。

### 智能指针

    智能指针是C++11中引入的一种新的内存管理机制，用于自动管理动态分配的内存，避免内存泄漏和悬空指针的问题。C++11中提供了三种智能指针：unique_ptr、shared_ptr和weak_ptr。
    智能指针在多线程中也是不安全的，只有只读访问智能指针(详看weak_ptr的lock())在多线程下是安全的

####  unique_ptr

    一个unique_ptr只能指向一个对象，当unique_ptr被销毁时，它所指向的对象也会被销毁。没有==运算符，不能复制unique_ptr，但可以移动unique_ptr，move()。

    通过排序算法，确定类引用、指针引用、智能指针引用的速度，在Debug模式下，速度差别不大，在Release模式下，智能指针引用速度最快。
    my_unique_ptr/my_unique_ptr_test.cpp

#### shared_ptr

    与unique_ptr不同，shared_ptr可以指向多个对象。
    shared_ptr通过引用计数来管理对象的生命周期，当引用计数为0时，对象会被销毁。
    在类的多态行为下，对智能指针shared_ptr进行转换
    dynamic_pointer_cast
    static_pointer_cast
    const_pointer_cast
    shared_ptr如果出现循环引用情况时，会出现内存无法自动释放情况，应该避免这种情况。

    my_shared_ptr/my_shared_ptr_test.cpp

#### weak_ptr

    weak_ptr不能单独使用，必须结合shared_ptr使用，
    weak_ptr的作用是对shared_ptr的对象进行观测，不会引起shared_ptr的引用计数的增加。基于这个特性，可以避开shared_ptr在循环引用情况下，出现内容无法自动释放情况
    先声明出一个shared_ptr后，由weak_ptr来作引用，例如将shared_ptr传入到weak_ptr的构造，如下：
    class A{};
    shared_ptr<A> sp1 = make_shared<A>();
    weak_ptr<A> wp1(sp1);
    weak_ptr<A> wp2;
    wp2 = wp1;
    std::shared_ptr<const int> wpConst = wp2.lock(); // 转换为只读访问
    可以通过weak_ptr的lock函数来获取share_ptr的管理权，且是原子操作，可保证在多线程操作中，对share_ptr的操作是安全有效的(只读访问智能指针)

    my_weak_ptr/my_weak_ptr_test.cpp

### Lambda表达式

    常见样式：
    [捕获变量](参数列表) 可选限定符->返回类型{
        //函数代码
    }
    捕获变量的方式 [=] 按值捕获 ;[&] 按引用捕获

    my_lambda/my_lambda_test.cpp

### 互斥和锁 mutex & lock

    这两个都是为多线程处理数据时，保证数据的一致性的。
    但是容易出现死锁和活锁的情况，避免出现的方法遵循以下原则：
    1、有多个锁进行lock的时候，保证所有线程中的lock都是按一个顺序来的，或者使用std::scoped_lock来保证顺序
    2、尽量的按照类或者方法都是为数据服务的理念，尽量的在一个类或者方法中，线程的运行只对一个锁进行操作。

### 读写锁 shared_mutex & shared_lock

    c++17中才有的
    shared_mutex是读写锁的一种，用来保证多线程操作时的数据一致性，避免出现数据竞争的情况。
    shared_mutex有两种模式：独占模式和共享模式。独占模式下，只有一个线程可以占有；共享模式下，可以有多个线程占有。
    shared_mutex是可重入的，可以同时被多个线程持有，但是只有一个线程可以持有写锁，其他线程只能持有读锁。

    shared_lock是读写锁的一种，用来保证多线程操作时的数据一致性，避免出现数据竞争的情况。
    shared_lock是可重入的，可以同时被多个线程持有，但是只有一个线程可以持有写锁，其他线程只能持有读锁。
    使用示例：
    //独占模式
    std::shared_mutex mutex;
    std::unique_lock<std::shared_mutex> lock(mutex);//也可以使用lock_guard
    lock.lock();
    // do something
    lock.unlock();

    //共享模式
    std::shared_mutex mutex;
    std::shared_lock<std::shared_mutex> lock(mutex);//只可以使用这个
    lock.lock_shared();
    // do something

    lock.unlock_shared();

    my_shared_mutex/my_shared_mutex_test.cpp 里面有互斥锁 std::mutex和读写锁std::shared_mutex，
    当使用互斥锁的时候，当写线程不占有锁时读线程之间也在争夺锁；而读写锁的时候，读线程可以在写线程不占有锁的时候同时读。提高了效率
    所以读写锁，广泛的应用在数据库、内容或媒体分发等方面

    与std::mutex存在std::timed_mutex一样，std::shared_mutex也存在std::shared_timed_mutex，std::shared_timed_mutex是std::shared_mutex的子类，提供了一种带有超时的读写锁。

### future/promise/async/packaged_task

    future和promise是配合使用的，promise用来设置future的值，future用来获取promise的值，并且future的值只能获取一次，获取后promise的值会被清空。
    async用来异步执行函数，返回一个future，future的值就是函数的返回值。
    packaged_task用来包装一个函数，返回一个future，future的值就是函数的返回值。
    使用示例：
    //promise & future
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    std::thread t([](std::promise<int> &promise){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        promise.set_value(1);
    }, std::ref(promise));
    std::cout << future.get() << std::endl;
    t.join();

    //async & future
    std::future<int> future = std::async([](){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return 1;
    });
    std::cout << future.get() << std::endl;

    //packaged_task & future
    std::packaged_task<int()> task([](){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return 1;
    });
    std::future<int> future = task.get_future();
    std::thread t(std::move(task));
    std::cout << future.get() << std::endl;
    t.join();

### atomic

    atomic是C++11中引入的原子操作类，用来保证多线程操作时的数据一致性，避免出现数据竞争的情况。
    atomic的常用操作包括：
    - load()：加载原子变量的值
    - store()：存储原子变量的值
    使用示例
    std::atomic<int> count(0);
    std::thread t1([&count](){
        for (int i = 0; i < 1000000; i++) {
            count.fetch_add(1);
        }
    });
    std::thread t2([&count](){
        for (int i = 0; i < 1000000; i++) {
            count.fetch_add(1);
        }
    });
    t1.join();
    t2.join();
    std::cout << count << std::endl;

### condition_variable

    condition_variable是C++11中引入的条件变量，用于线程间的同步。它通常与互斥锁一起使用，用于等待某个条件成立。
    condition_variable的常用操作包括：
    - wait()：等待条件变量，直到条件成立
    - notify_one()：唤醒一个等待的线程
    - notify_all()：唤醒所有等待的线程
    使用示例
    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;
    std::thread t1([&](){
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&](){ return ready; });
        std::cout << "t1: " << ready << std::endl;
    });
    std::thread t2([&](){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<std::mutex> lock(mtx);
        ready = true;
        cv.notify_one();
        std::cout << "t2: " << ready << std::endl;
    });
    t1.join();
    t2.join();
