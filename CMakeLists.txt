cmake_minimum_required(VERSION 3.10)
project(helloCplex)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
link_directories(lib/x86-64_linux/static_pic)

add_definitions(-DIL_STD)

file(GLOB HGJPathSmoothFILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        include/HGJPathSmooth/*.cpp include/HGJPathSmooth/*.h)

add_executable(helloCplex main.cpp)
add_executable(lp ilolpex1.cpp)
add_library(HGJ_Path_Smooth ${HGJPathSmoothFILES})

target_link_libraries(lp ilocplex cplex concert m pthread dl)
target_link_libraries(helloCplex ilocplex cplex concert m pthread dl)