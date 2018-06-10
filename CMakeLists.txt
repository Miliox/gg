cmake_minimum_required (VERSION 2.8)

# projectname is the same as the main-executable
project(gg)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_definitions('-g')
add_definitions('-Wall')
add_definitions('-std=c++17')

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB SOURCES src/*.cpp test/*.cpp)
include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
