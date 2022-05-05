cmake_minimum_required(VERSION 3.0)

project(algo-demo-II-1102)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB IMGUI "src/imgui/imgui*.cpp")
file(GLOB BACKEND "src/imgui/backend/*.cpp")

set(SOURCES
    src/main.cpp
    ${IMGUI}
    ${BACKEND}
)

add_executable(main ${SOURCES})

target_link_libraries(main glfw)
target_link_libraries(main OpenGL::GL)

include_directories(
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include/imgui
        ${PROJECT_SOURCE_DIR}/include/imgui/backend
)
