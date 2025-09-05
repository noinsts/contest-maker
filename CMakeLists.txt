cmake_minimum_required(VERSION 3.26)

project(contest-maker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

file(GLOB SRC_FILES src/*.cpp)

add_executable(main ${SRC_FILES})


target_include_directories(main PRIVATE
    ${GTKMM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(main PRIVATE ${GTKMM_LIBRARIES})
