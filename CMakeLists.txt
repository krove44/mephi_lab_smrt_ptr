cmake_minimum_required(VERSION 3.20)
project(mephi_lab_smrt_ptr)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -O0")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(mephi_lab_smrt_ptr
        main.cpp
        ptr/uniq_ptr.h
        ptr/shared_ptr.h
        ptr/control_block.h
        ptr/weak_ptr.h
)


enable_testing()
add_executable(run_tests
        tests/shared_ptr_basic_test.cpp
        tests/shared_ptr_poly_test.cpp
        tests/weak_ptr_test.cpp
        tests/bench_small_test.cpp
        tests/bench_large_test.cpp
        tests/bench_copy_test.cpp
        tests/uniq_ptr_test.cpp
        tests/bench_large_test.cpp
)
target_link_libraries(run_tests PRIVATE GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)