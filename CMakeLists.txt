# Minimum required version of CMake
cmake_minimum_required(VERSION 3.14)

# Project name and C++ standard
project(mgclient LANGUAGES CXX)

# Use C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Add the executable target and link the source files
add_executable(mazeclient
    MazeClient.cpp
)

# Find and link necessary libraries (CPR, Boost, etc.)
find_package(Boost REQUIRED COMPONENTS system thread)

# Link libraries to the target executable
target_link_libraries(mazeclient
    Boost::system
    Boost::thread
)

# Specify where the target binary should be placed after build
set_target_properties(mazeclient PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
