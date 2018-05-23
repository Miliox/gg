cmake_minimum_required (VERSION 2.8)

# projectname is the same as the main-executable
project(gg)

add_definitions('-g')
add_definitions('-Wall')
add_definitions('-std=c++17')

file(GLOB SOURCES src/*.cpp)
include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES})
