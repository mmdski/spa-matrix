cmake_minimum_required(VERSION 3.14.0)
project(spamatrix VERSION 0.1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -pedantic -Werror)

message(STATUS "System name: ${CMAKE_SYSTEM_NAME}")
message(STATUS "C Compiler ID: ${CMAKE_C_COMPILER_ID}")
message(STATUS "CXX Compiler ID: ${CMAKE_CXX_COMPILER_ID}")

# add debug and ASAN flags for ASAN build
if("${CMAKE_BUILD_TYPE}" STREQUAL "Asan")
    # determine if ASan is available
    try_compile(ASAN_AVAILABLE "${CMAKE_BINARY_DIR}"
        SOURCES "${CMAKE_SOURCE_DIR}/san-check/no-leak.c"
        CMAKE_FLAGS "-fsanitize=address"
        LINK_OPTIONS "-fsanitize=address"
        COPY_FILE "${CMAKE_BINARY_DIR}/lsan-check/no-leak")
    message(STATUS "ASan available: ${ASAN_AVAILABLE}")

    # determine if LSan is available with ASan
    set(LSAN_AVAILABLE FALSE)

    if(ASAN_AVAILABLE)
        file(COPY_FILE "${CMAKE_SOURCE_DIR}/san-check/lsan-check.sh" "${CMAKE_BINARY_DIR}/lsan-check/lsan-check.sh")
        set(ENV{SOURCE_DIR} ${CMAKE_SOURCE_DIR})

        execute_process(COMMAND
            "${CMAKE_BINARY_DIR}/lsan-check/lsan-check.sh"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/lsan-check"
            RESULT_VARIABLE LSAN_CHECK_RESULT
            OUTPUT_QUIET ERROR_QUIET)

        if(LSAN_CHECK_RESULT STREQUAL "0")
            set(LSAN_AVAILABLE TRUE)
        endif()
    endif()

    message(STATUS "ASan+LSan available: ${LSAN_AVAILABLE}")

    # determine if UBSan is available
    try_compile(UBSAN_AVAILABLE "${CMAKE_BINARY_DIR}"
        SOURCES "${CMAKE_SOURCE_DIR}/san-check/no-leak.c"
        CMAKE_FLAGS "-fsanitize=undefined"
        LINK_OPTIONS "-fsanitize=undefined")
    message(STATUS "UBSan available: ${UBSAN_AVAILABLE}")

    add_compile_options(-g -fno-omit-frame-pointer -fsanitize=address)
    add_link_options(-fsanitize=address)

    # -fno-sanitize-recover=all is necessary for UBSAN test failure
    if(UBSAN_AVAILABLE)
        add_compile_options(-fsanitize=undefined -fno-sanitize-recover=all)
        add_link_options(-fsanitize=undefined)
    endif()
endif()

include_directories(include)

add_subdirectory(src)

# build the files in the app subdirectory
# add_subdirectory(app)
if(TESTS)
    message(STATUS "Building tests")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    include(GoogleTest)

    add_subdirectory(tests)
endif()
