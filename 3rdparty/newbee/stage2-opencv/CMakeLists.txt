cmake_minimum_required(VERSION 3.5.0)
project(games VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
file(GLOB SRCFILES "src/*.cpp")
add_executable(games ${SRCFILES} todo.cpp)
target_link_libraries(games ${OpenCV_LIBS})