cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_COMPILER "/usr/bin/gcc-14" CACHE PATH "C compiler" FORCE)
set(CMAKE_CXX_COMPILER "/usr/bin/g++-14" CACHE PATH "C++ compiler" FORCE)

project(lab1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(USE_SMC "Use SMC implementation for parser" OFF)
add_compile_options(-O0)

# Добавляем поддиректории с явным указанием бинарных директорий
add_subdirectory(../src/interfaces ./interfaces_build)
add_subdirectory(../src/impls ./impls_build)
add_subdirectory(../src/service ./service_build)

# Явно указываем исходные файлы для lex_version
set(LEX_VERSION_SOURCES
        ../src/impls/lex_version/source/generated.cpp
)

# Явно указываем исходные файлы для SMC версии
set(SMC_VERSION_SOURCES
        ../src/impls/smc_version/parser_sm.cpp
)

add_executable(timing
        main.cpp
        ${LEX_VERSION_SOURCES}
        ${SMC_VERSION_SOURCES}
)

target_include_directories(timing PRIVATE
        ../src/impls/lex_version/include
        ../src/interfaces/include
        ../src/impls/regex_version/include
        ../src/impls/smc_version
)

target_link_libraries(timing PRIVATE
        service
        impls
        interfaces
        # Добавляем библиотеки flex если используются
        fl
)