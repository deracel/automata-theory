cmake_minimum_required (VERSION 3.28)
set(CMAKE_C_COMPILER "/usr/bin/gcc-14" CACHE PATH "C compiler" FORCE)
set(CMAKE_CXX_COMPILER "/usr/bin/g++-14" CACHE PATH "C++ compiler" FORCE)

project(tests)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra")
option(ENABLE_COVERAGE "Enable code coverage" OFF)
get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    message(STATUS "Code coverage enabled")
endif()

if(ENABLE_COVERAGE)
    find_program(LCOV_PATH lcov REQUIRED)
    find_program(GENHTML_PATH genhtml REQUIRED)
    find_program(GCOV_PATH gcov-14 REQUIRED)
    find_program(XDG_OPEN xdg-open REQUIRED)

    message(STATUS "Found lcov: ${LCOV_PATH}")
    message(STATUS "Found genhtml: ${GENHTML_PATH}")
    message(STATUS "Found gcov: ${GCOV_PATH}")
    message(STATUS "Found xdg-open: ${XDG_OPEN}")

    if(LCOV_PATH AND GENHTML_PATH AND GCOV_PATH AND XDG_OPEN)
        add_custom_target(coverage
                COMMAND echo "=== Cleaning old coverage data ==="
                COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} --directory . --zerocounters

                COMMAND echo "=== Running tests ==="
                COMMAND $<TARGET_FILE:tests> --gtest_output=xml:test_results.xml

                COMMAND echo "=== Capturing coverage ==="
                COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} --directory . --capture --output-file coverage.info --ignore-errors mismatch

                COMMAND echo "=== Keeping ONLY project files ==="
                COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} --extract coverage.info "${PROJECT_ROOT}/src/*" --output-file coverage_filtered.info --ignore-errors mismatch

                COMMAND echo "=== Generating HTML report ==="
                COMMAND ${GENHTML_PATH} coverage_filtered.info --output-directory ${CMAKE_BINARY_DIR}/coverage_report --ignore-errors mismatch

                COMMAND echo "=== Coverage report generated! ==="
                COMMAND echo "HTML report: ${CMAKE_BINARY_DIR}/coverage_report/index.html"

                COMMAND ${XDG_OPEN} ${CMAKE_BINARY_DIR}/coverage_report/index.html

                COMMAND echo "=== Browser should open now ==="

                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                DEPENDS tests
                COMMENT "Generating and opening code coverage report"
                VERBATIM
        )
    else()
        message(FATAL_ERROR "Required programs not found: lcov, genhtml, gcov-14, or xdg-open")
    endif()
endif()


find_package(GTest REQUIRED)
include (GoogleTest)

add_subdirectory(../src/interfaces ${CMAKE_BINARY_DIR}/interfaces)
add_subdirectory(../src/impls ${CMAKE_BINARY_DIR}/impls)
add_subdirectory(../src/parser ${CMAKE_BINARY_DIR}/parser)

add_executable(tests tests.cpp)

target_link_libraries(tests PRIVATE
        parser
        impls
        interfaces
        GTest::gtest_main

)

gtest_discover_tests(tests)