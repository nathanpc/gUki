### CMakeList.txt
### CMake definitions for the gUki application.
###
### Author: Nathan Campos <hi@nathancampos.me>

# Determine the minimum CMake version.
cmake_minimum_required(VERSION 3.0)

# Setup the project.
project(gUki VERSION 1.0.0 LANGUAGES C)
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-Wall -Wextra -pedantic -Wno-deprecated-declarations
	-Wno-unused-parameter)
#-DGTK_DISABLE_SINGLE_INCLUDES -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED)

# Setup GTK+ and WebKitGTK.
find_package(PkgConfig REQUIRED)
if(GTK_VERSION EQUAL 3)
	pkg_check_modules(GTK REQUIRED gtk+-3.0)
	pkg_check_modules(WEBKIT REQUIRED webkit2gtk-4.0)
elseif(GTK_VERSION EQUAL 2)
	pkg_check_modules(GTK REQUIRED gtk+-2.0)
	pkg_check_modules(WEBKIT REQUIRED webkit-1.0)
else()
	message(FATAL_ERROR "No GTK+ version provided. Please run the cmake command with the GTK_VERSION variable set. (eg. \"cmake .. -DGTK_VERSION=3\")")
endif()
include_directories(${GTK_INCLUDE_DIRS} ${WEBKIT_INCLUDE_DIRS})

# Setup the files.
file(GLOB SOURCES "src/*.c")

# Setup link and include paths.
set(LIBUKI_DIST_DIR ${CMAKE_SOURCE_DIR}/../libuki/build/dist)
include_directories(${LIBUKI_DIST_DIR}/include)
link_directories(${LIBUKI_DIST_DIR}/lib)

# Build our executable.
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} libuki.so ${CMAKE_THREAD_LIBS_INIT}
	${GTK_LIBRARIES} ${WEBKIT_LIBRARIES})

# Set properties.
set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION ${PROJECT_VERSION})

# Configure the installation.
install(
	TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin)
