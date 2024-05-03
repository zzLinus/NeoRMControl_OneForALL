cmake_minimum_required(VERSION 3.25)
project(new_infantry)

#set(CMAKE_C_COMPILER clang++)
#set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 20)

include_directories(./3rdparty/include)
include_directories(./include)

add_compile_options(-D__DEBUG__)
add_compile_options(-Wno-pointer-arith -Wno-unused-parameter)

file(GLOB_RECURSE SOURCE src/main.cc ./src/*.cc)

add_executable(${PROJECT_NAME} ${SOURCE})

execute_process(
        COMMAND make
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/src-serial/)

execute_process(
        COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/src-serial/build/libserial.a ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/lib/
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_directories(${PROJECT_NAME} PUBLIC ./3rdparty/lib)
target_link_libraries(${PROJECT_NAME} serial)
