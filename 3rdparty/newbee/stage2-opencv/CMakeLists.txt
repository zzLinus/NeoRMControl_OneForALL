cmake_minimum_required(VERSION 3.5.0)
project(games VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} inc)

file(GLOB SRCFILES "src/*.cpp" "src/framework/*.cpp")
add_executable(games ${SRCFILES})

target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS})