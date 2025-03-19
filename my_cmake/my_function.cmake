function(my_function a b)
    message("my_function")
    message(STATUS "a: ${a}")
    message(STATUS "b: ${b}")
endfunction()

function(my_function1 a b)
    message("my_function1")
    message(STATUS "a: ${a}")
    message(STATUS "b: ${b}")
    message("ARGC: ${ARGC}")
    message("ARGV: ${ARGV}")
    message("ARGS: ${ARGN}")
endfunction()

function(my_function2 target)
    set(options USE_MYLIB)
    set(oneValueArgs MYLIB_PATH)
    set(multiValueArgs SOURCES INCLUDES)
    cmake_parse_arguments(arg_MY_FUNCTION2 "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    message("my_function2")
    message(STATUS "target: ${target}")
    message("
    USE_MYLIB:${arg_MY_FUNCTION2_USE_MYLIB}}
    MYLIB_PATH:${arg_MY_FUNCTION2_MYLIB_PATH}
    SOURCES:${arg_MY_FUNCTION2_SOURCES}
    INCLUDES:${arg_MY_FUNCTION2_INCLUDES}")
    message("UNPARSED_ARGUMENTS:${arg_MY_FUNCTION2_UNPARSED_ARGUMENTS}")
    message("KEYWORDS_MISSING_VALUES:${arg_MY_FUNCTION2_KEYWORDS_MISSING_VALUES}")
endfunction()