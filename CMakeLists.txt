cmake_minimum_required(VERSION 3.2)
project(engine VERSION 0.1.0)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(CMAKE_BUILD_TYPE "Debug")

set(OpenGL_GL_PREFERENCE "GLVND")

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_subdirectory(extern)

add_subdirectory(src)
