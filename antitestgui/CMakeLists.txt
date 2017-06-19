cmake_minimum_required(VERSION 3.1.0)

project(antitestgui)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets)
find_package(Qt5Network)

FILE(GLOB_RECURSE src src/*.cpp src/*.h src/*.ui)

add_executable(
    ${PROJECT_NAME}
    ${src}
)

target_link_libraries(
    ${PROJECT_NAME}
    Qt5::Widgets
    Qt5::Network
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy
    "$<TARGET_FILE:${PROJECT_NAME}>"
    "${CMAKE_SOURCE_DIR}/bin/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
)
