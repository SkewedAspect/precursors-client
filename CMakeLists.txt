# Set minimum cmake version
cmake_minimum_required(VERSION 2.8.11)

# Name our project
project(PrecursorsClient)


# Add our CMake modules to the path.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Add OGRE's CMake modules to the path.
if(WIN32)
	set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/cmake/;${CMAKE_MODULE_PATH}")

elseif(UNIX)
	if(EXISTS "$ENV{OGRE_HOME}/cmake")
		set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/cmake/;${CMAKE_MODULE_PATH}")

	elseif(EXISTS "/usr/local/lib/OGRE/cmake")
		set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")

	elseif(EXISTS "/usr/lib/OGRE/cmake")
		set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")

	else()
		message(SEND_ERROR "Failed to find module path.")

	endif()
endif()


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

# Find QCA2
#find_package(QCA2 REQUIRED)

#if(NOT QCA2_FOUND)
#	message(SEND_ERROR "Failed to find QCA2.")
#endif()

# Find OGRE
find_package(OGRE REQUIRED)

# Find OIS
find_package(OIS REQUIRED)

if(NOT OIS_FOUND)
	message(SEND_ERROR "Failed to find OIS.")
endif()

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find Boost
if(NOT OGRE_BUILD_PLATFORM_IPHONE)
	if(WIN32 OR APPLE)
		set(Boost_USE_STATIC_LIBS TRUE)
	else()
		# Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
		set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
	endif()

	if(MINGW)
		# This is probably a bug in CMake: the boost find module tries to look for
		# boost libraries with name libboost_*, but CMake already prefixes library
		# search names with "lib". This is the workaround.
		set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "")
	endif()

	set(Boost_ADDITIONAL_VERSIONS
		"1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40"
		"1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37"
		)

	# Components that need linking (NB does not include header-only components like bind)
	set(OGRE_BOOST_COMPONENTS thread date_time system)

	find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} REQUIRED)
	if(NOT Boost_FOUND)
		message("Failed to find dynamic Boost libs; trying static Boost libs instead...")

		# Try again with the other type of libs
		set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
		find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} REQUIRED)

		if(NOT Boost_FOUND)
			message(SEND_ERROR "Failed to find static or dynamic Boost libs!")
		endif()
	endif()

	# Set up references to Boost
	add_definitions(-DBOOST_ALL_NO_LIB)
	include_directories(
		${Boost_INCLUDE_DIR}
		)

	set(OGRE_LIBRARIES
		${OGRE_LIBRARIES}
		${Boost_LIBRARIES}
		)
endif()


include_directories(
	${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	#${QCA2_INCLUDE_DIR}
	)


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
