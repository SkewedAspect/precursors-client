FIND_PACKAGE(OpenSSL)

# List project files
set(PCHANNELS_HEADERS
	pchannels.h
	pchannels_global.h
	pchannelsrequest.h
	qnetstring.h
	aes.h
   	${CMAKE_CURRENT_BINARY_DIR}/../config.h
	)
set(PCHANNELS_SOURCES
	pchannels.cpp
	pchannelsrequest.cpp
	qnetstring.cpp
	aes.cpp
	)

add_library(PChannels SHARED
	${PCHANNELS_HEADERS}
	${PCHANNELS_SOURCES}
	)

set_target_properties(PChannels PROPERTIES COMPILE_FLAGS -fPIC)
set_target_properties(PChannels PROPERTIES DEBUG_POSTFIX _d)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${OPENSSL_INCLUDE_DIR}
	)

qt5_use_modules(PChannels
	Core
	Network
	)

# Tell CMake what to link with
target_link_libraries(PChannels
	#TODO: Commented until macports updates to CMake 2.8.11 or newer.
	#Qt5::Core
	#Qt5::Network

	${OPENSSL_LIBRARIES}

	PLogging
	PreUtil
	)


# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command(TARGET PChannels POST_BUILD
		COMMAND copy \"$(TargetPath)"\"" .\\dist\\bin
		)
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX)
	set(ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dist/bin)
	set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR UNIX)


if(WIN32)
	install(TARGETS PChannels
		LIBRARY DESTINATION bin
		CONFIGURATIONS All)
elseif(UNIX)
	install(TARGETS PChannels
		LIBRARY DESTINATION bin
		CONFIGURATIONS All)
endif()
