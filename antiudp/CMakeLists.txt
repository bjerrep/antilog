cmake_minimum_required(VERSION 3.1.0)

project(antiudp)

find_package(Qt5Network)

add_executable(
    ${PROJECT_NAME} 
    src/main.cpp
)

target_link_libraries(
    ${PROJECT_NAME} 
    Qt5::Network
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy
    "$<TARGET_FILE:${PROJECT_NAME}>"
    "${CMAKE_SOURCE_DIR}/bin/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
)
