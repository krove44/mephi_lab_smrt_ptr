cmake_minimum_required(VERSION 3.20)
project(mephi_lab_smrt_ptr)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -g")

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
)


enable_testing()
add_executable(run_tests
        tests/shared_ptr_test.cpp
        tests/uniq_ptr_test.cpp
)
target_link_libraries(run_tests PRIVATE GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)