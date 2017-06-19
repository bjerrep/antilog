cmake_minimum_required(VERSION 3.1.0)

project(antilog)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets)
find_package(Qt5Network)

file(GLOB src src/*.cpp src/*.h src/*.ui *.qrc)

add_executable(
    ${PROJECT_NAME}
    ${src}
    )

set(CMAKE_CXX_FLAGS "-fno-elide-constructors -pedantic-errors -ansi")
set(CMAKE_CXX_FLAGS " -Wextra -Wall -Winit-self -Wold-style-cast -Woverloaded-virtual")
set(CMAKE_CXX_FLAGS " -Wuninitialized -Wmissing-declarations -std=c++98")

add_definitions(
    -DQT_USE_FAST_CONCATENATION
    -DQT_USE_FAST_OPERATOR_PLUS
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
