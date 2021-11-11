cmake_minimum_required(VERSION 3.1.0)

project(antilog)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

file(GLOB_RECURSE src src/*.cpp src/*.h src/*.ui *.qrc)

add_executable(
    ${PROJECT_NAME}
    ${src}
    )

include_directories(
    src
    )

set(CMAKE_AUTOUIC_SEARCH_PATHS src/format)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-elide-constructors -pedantic-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wall -Winit-self -Wold-style-cast -Woverloaded-virtual")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuninitialized -Wmissing-declarations -std=c++98")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-braces -Wmissing-field-initializers")
endif()

add_definitions(
    -DQT_USE_FAST_CONCATENATION
    -DQT_USE_FAST_OPERATOR_PLUS
    -DQTDESIGNER_EXPORT_WIDGETS
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
