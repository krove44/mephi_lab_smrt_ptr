cmake_minimum_required(VERSION 4.2)
project(mephi_lab_smrt_ptr)

set(CMAKE_CXX_STANDARD 23)

add_executable(mephi_lab_smrt_ptr main.cpp
        cmake-build-debug/smrt_ptr.h)
