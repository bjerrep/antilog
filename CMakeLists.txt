cmake_minimum_required(VERSION 3.1.0)

project(antilogviewer)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(antilog antilog/build)
add_subdirectory(antitestgui antitestgui/build)
add_subdirectory(antiudp antiudp/build)

add_custom_target( doc ALL
  SOURCES README.md
)
