cmake_minimum_required(VERSION 3.25)

project(SmallToolBox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(directory "common/smallToolBox")
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${directory}")
    message(STATUS "${directory} exists ...")
else()
    include(FetchContent)
    FetchContent_Declare(
        smallToolBox
        GIT_REPOSITORY https://github.com/jpenrici/Small_ToolBox.git
        GIT_TAG "main"
        GIT_SHALLOW TRUE
        GIT_PROGRESS ON
        SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/${directory}"
    )
    FetchContent_MakeAvailable(smallToolBox)
endif()
