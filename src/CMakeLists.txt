cmake_minimum_required(VERSION 3.22)
project(raylibStart LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add raylib as a subdirectory (source in 3rdParty, build in build/raylib)
add_subdirectory(../3rdParty/raylib ${CMAKE_BINARY_DIR}/raylib)

# Create executable
add_executable(${PROJECT_NAME} main.cpp)

# Link raylib
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
