cmake_minimum_required(VERSION 3.15)
project(gtest-parse)

set(HOME ${PROJECT_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${HOME}/bin/)
include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_SYSTEM_NAME Linux)
set(C_SOURCE ${HOME}/src/parse.cpp)
add_library(c_objects OBJECT ${C_SOURCE})
add_executable(gtest-parse $<TARGET_OBJECTS:c_objects>)

enable_testing()
target_link_libraries(gtest-parse
    gtest
    gtest_main
    pthread
)