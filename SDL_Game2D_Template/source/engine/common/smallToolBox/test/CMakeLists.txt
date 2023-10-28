cmake_minimum_required(VERSION 3.25)

project(SmallToolBox_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "*.cpp")

set(RESOURCE_FILES
    resources/svg_commands.txt
)

include_directories("../src/")

foreach(testSourceFile ${SOURCES})
    get_filename_component(testName ${testSourceFile} NAME_WE)
    message(STATUS ${testName} " : " ${testSourceFile})
    add_executable(${testName} ${testSourceFile})
    install(TARGETS ${testName}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endforeach()

file(COPY ${RESOURCE_FILES} DESTINATION ${CMAKE_BINARY_DIR}/Resources)
