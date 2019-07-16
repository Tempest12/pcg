#author Shane del Solar

cmake_minimum_required(VERSION 3.0)

project(pcg)
set(CMAKE_CXX_COMPILIER g++)


add_subdirectory(src)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*/*.cpp)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -pedantic -O0 -g")

add_executable(pcg ${SOURCES})

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/include)

target_link_libraries(pcg ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES})
