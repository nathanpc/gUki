### CMakeList.txt
### CMake definitions for the Uki shared library.
###
### Author: Nathan Campos <hi@nathancampos.me>

# Determine the minimum CMake version.
cmake_minimum_required(VERSION 3.0)

# Setup the project.
project(gUki VERSION 1.0.0 LANGUAGES C)
set(CMAKE_BUILD_TYPE Debug)

# Setup the files.
file(GLOB SOURCES "src/*.c")

# Setup link and include paths.
include_directories(${CMAKE_SOURCE_DIR}/../uki/build/install/include)
link_directories(${CMAKE_SOURCE_DIR}/../uki/build/install/lib)

# Build our executable.
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} libuki.so)

# Set properties.
set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION ${PROJECT_VERSION})

# Configure the installation.
install(
  TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin)
