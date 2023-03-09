cmake_minimum_required(VERSION 3.5)

project(SDL_Test_03 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
)

add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
)

find_package(SDL2 REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries (
    ${PROJECT_NAME} PUBLIC
    SDL2::SDL2
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
