cmake_minimum_required(VERSION 3.25)

if(APPLE)
    execute_process(
            COMMAND xcrun --sdk macosx --show-sdk-path
            OUTPUT_VARIABLE CMAKE_OSX_SYSROOT
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
    link_directories(/opt/homebrew/lib)
endif()

project(GAME VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib REQUIRED)

find_library(LUA_LIBRARY
        NAMES lua liblua lua5.4 liblua5.4
        PATHS /opt/homebrew/lib
        NO_DEFAULT_PATH
        REQUIRED
        )

find_path(LUA_INCLUDE_DIR
        NAMES lua.h
        PATHS /opt/homebrew/include
        PATH_SUFFIXES lua lua5.4
        NO_DEFAULT_PATH
        REQUIRED
        )


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/external/rlImGui/rlImGui.cpp"
        "${PROJECT_SOURCE_DIR}/external/imGui/*.cpp"
        )

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
        ${LUA_INCLUDE_DIR}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/external
        ${PROJECT_SOURCE_DIR}/external/rlImGui
        ${PROJECT_SOURCE_DIR}/external/imGui
        ${PROJECT_SOURCE_DIR}/external/FastNoise
        )

target_link_libraries(${PROJECT_NAME}
        raylib
        ${LUA_LIBRARY}
        )