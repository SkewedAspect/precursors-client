# List project files
set(QCHANNELS_HEADERS
	qchannels.h
	qchannels_global.h
	qchannelsrequest.h
	qnetstring.h
   	${CMAKE_CURRENT_BINARY_DIR}/../config.h
	)
set(QCHANNELS_SOURCES
	qchannels.cpp
	qchannelsrequest.cpp
	qnetstring.cpp
	)

add_library(QChannels SHARED
	${QCHANNELS_HEADERS}
	${QCHANNELS_SOURCES}
	)

set_target_properties(QChannels PROPERTIES COMPILE_FLAGS -fPIC)
set_target_properties(QChannels PROPERTIES DEBUG_POSTFIX _d)

# Tell CMake what to link with
target_link_libraries(QChannels
	Qt5::Core
	Qt5::Network
	)


# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command(TARGET QChannels POST_BUILD
		COMMAND copy \"$(TargetPath)"\"" .\\dist\\bin
		)
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX)
	set(ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dist/bin)
	set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR UNIX)


if(WIN32)
	install(TARGETS QChannels
		LIBRARY DESTINATION bin
		CONFIGURATIONS All)
elseif(UNIX)
	install(TARGETS QChannels
		LIBRARY DESTINATION bin
		CONFIGURATIONS All)
endif()
