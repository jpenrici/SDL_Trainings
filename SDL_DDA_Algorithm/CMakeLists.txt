cmake_minimum_required(VERSION 3.5)

project(SDL_DDA_algorithm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

include_directories(
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries (
    ${PROJECT_NAME} PUBLIC
    SDL2::SDL2
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
