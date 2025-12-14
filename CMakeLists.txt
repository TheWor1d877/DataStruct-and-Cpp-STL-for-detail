cmake_minimum_required(VERSION 4.1)

project(Data_Struct)

set(CMAKE_CXX_STANDARD 20)

add_executable(ds
        src/main.cpp
        src/SkipList.cpp
        src/testMenu.cpp
)

target_include_directories(ds PUBLIC ${CMAKE_SOURCE_DIR}/include)