cmake_minimum_required(VERSION 4.1)

project(Data_Struct)

set(CMAKE_CXX_STANDARD 20)

add_executable(ds
        src/main.cpp
        src/SkipList.cpp
        benchmark/SkipList_test.cpp
)
add_compile_options(
        "$<$<CONFIG:RELEASE>:-O3>"
        "$<$<CONFIG:RELWITHDEBINFO>:-O2 -g>"
        "$<$<CONFIG:MINSIZEREL>:-O1>"
        "$<$<CONFIG:DEBUG>:-O0 -g3>"
)
target_include_directories(ds PUBLIC ${CMAKE_SOURCE_DIR}/include)