cmake_minimum_required(VERSION 3.22)
project(AnimeTower LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option to build without console window (for distribution)
option(BUILD_NO_CONSOLE "Build without console window (Windows GUI subsystem)" OFF)

# Add raylib as a subdirectory (source in 3rdParty, build in build/raylib)
add_subdirectory(../3rdParty/raylib ${CMAKE_BINARY_DIR}/raylib)

# Create executable
add_executable(${PROJECT_NAME} main.cpp)

# Link raylib
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Set Windows GUI subsystem when building without console
if(WIN32 AND BUILD_NO_CONSOLE)
    target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")
    message(STATUS "Building without console window (GUI subsystem)")
endif()
