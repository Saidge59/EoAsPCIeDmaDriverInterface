cmake_minimum_required(VERSION 3.10)
project(DriverProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/../include)

add_executable(driver_test src/main.cpp src/driver_interface.cpp)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(driver_test PRIVATE -Wall -Wextra -Wpedantic)
endif()
