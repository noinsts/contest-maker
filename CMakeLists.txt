cmake_minimum_required(VERSION 3.26)
project(contest-maker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

# GTKMM
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

# GtkSourceView
pkg_check_modules(GTK_SOURCE REQUIRED gtksourceviewmm-3.0)

# JSON
find_package(nlohmann_json 3.11 REQUIRED)

# Source files
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Executable
add_executable(main ${SRC_FILES})

# Include directories
target_include_directories(main PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${GTKMM_INCLUDE_DIRS}
        ${GTK_SOURCE_INCLUDE_DIRS}
)

# Libraries
target_link_libraries(main PRIVATE
        ${GTKMM_LIBRARIES}
        ${GTK_SOURCE_LIBRARIES}
        nlohmann_json::nlohmann_json
)
