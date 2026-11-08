cmake_minimum_required(VERSION 3.22)
project(AnimeTower LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add raylib as a subdirectory (source in 3rdParty, build in build/raylib)
add_subdirectory(../3rdParty/raylib ${CMAKE_BINARY_DIR}/raylib)

# =============================================================================
# Function: add_game_executable
# Creates an executable target with common settings (raylib, console options)
#
# Usage:
#   add_game_executable(<target_name> <main_file> [NO_CONSOLE])
#
# Examples:
#   add_game_executable(AnimeTower main.cpp)
#   add_game_executable(AnimeTower_Tests main_Tests.cpp)
#   add_game_executable(AnimeTower_Dist main.cpp NO_CONSOLE)
# =============================================================================
function(add_game_executable TARGET_NAME MAIN_FILE)
    set(options NO_CONSOLE)
    cmake_parse_arguments(PARSE_ARGV 2 ARG "${options}" "" "")
    
    # Create executable
    add_executable(${TARGET_NAME} ${MAIN_FILE})
    
    # Link raylib
    target_link_libraries(${TARGET_NAME} PRIVATE raylib)
    
    # Set Windows GUI subsystem when building without console
    if(WIN32 AND ARG_NO_CONSOLE)
        target_link_options(${TARGET_NAME} PRIVATE "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")
        message(STATUS "${TARGET_NAME}: Building without console window (GUI subsystem)")
    endif()
endfunction()

# =============================================================================
# Game executable
# =============================================================================
option(BUILD_NO_CONSOLE "Build game without console window (Windows GUI subsystem)" OFF)

if(BUILD_NO_CONSOLE)
    add_game_executable(${PROJECT_NAME} main.cpp NO_CONSOLE)
else()
    add_game_executable(${PROJECT_NAME} main.cpp)
endif()

# =============================================================================
# Tests executable
# =============================================================================
option(BUILD_TESTS_NO_CONSOLE "Build tests without console window (Windows GUI subsystem)" OFF)

if(BUILD_TESTS_NO_CONSOLE)
    add_game_executable(${PROJECT_NAME}_Tests main_Tests.cpp NO_CONSOLE)
else()
    add_game_executable(${PROJECT_NAME}_Tests main_Tests.cpp)
endif()
