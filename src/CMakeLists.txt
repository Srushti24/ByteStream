cmake_minimum_required(VERSION 3.20)
project(ByteStream)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build library from cpp
add_library(bytestream
    bytestream.cpp
)

# If you have a main.cpp to test
add_executable(test_app 
main.cpp)
target_link_libraries(test_app PRIVATE bytestream)