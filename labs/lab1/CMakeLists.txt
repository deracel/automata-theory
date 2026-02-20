cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_COMPILER "/usr/bin/gcc-14" CACHE PATH "C compiler" FORCE)
set(CMAKE_CXX_COMPILER "/usr/bin/g++-14" CACHE PATH "C++ compiler" FORCE)

project(lab1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/interfaces)
add_subdirectory(src/impls)
add_subdirectory(src/parser)

add_executable(app main.cpp)

target_link_libraries(app PRIVATE
        parser
        impls
        interfaces
)