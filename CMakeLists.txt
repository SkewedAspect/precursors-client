# Set minimum cmake version
cmake_minimum_required(VERSION 2.8.11)

# Name our project
project(PrecursorsClient)


# Add our CMake modules to the path.
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


if(CMAKE_BUILD_TYPE STREQUAL "")
	# CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
	# differentiation between debug and release builds.
	set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

set(CMAKE_DEBUG_POSTFIX "_d")

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)


# Find the Qt modules
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5OpenGL REQUIRED)

# Find OpenGL
find_package(OpenGL REQUIRED)

file(MAKE_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}/dist/bin
	)
file(MAKE_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}/dist/media
	)

# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command(TARGET QChannels PRE_BUILD
		COMMAND if not exist .\\dist\\bin mkdir .\\dist\\bin
		)
endif(WIN32 AND NOT MINGW)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
   	${CMAKE_CURRENT_BINARY_DIR}/config.h
	)


ADD_SUBDIRECTORY(qchannels qchannels)
ADD_SUBDIRECTORY(src src)
