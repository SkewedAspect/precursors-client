# List project files
set(PREUTIL_HEADERS
	preutil.h
	preutil_global.h
   	${CMAKE_CURRENT_BINARY_DIR}/../config.h
	)
set(PREUTIL_SOURCES
	preutil.cpp
	)

add_library(PreUtil SHARED
	${PREUTIL_HEADERS}
	${PREUTIL_SOURCES}
	)

set_target_properties(PreUtil PROPERTIES COMPILE_FLAGS -fPIC)
set_target_properties(PreUtil PROPERTIES DEBUG_POSTFIX _d)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}/..
)

# Tell CMake what to link with
target_link_libraries(PreUtil
	Qt5::Core
	Qt5::Network

	PLogging
	)

# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command(TARGET PreUtil POST_BUILD
		COMMAND copy \"$(TargetPath)"\"" .\\dist\\bin
		)
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX)
	set(ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dist/bin)
	set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR UNIX)


if(WIN32)
	install(TARGETS PreUtil
		LIBRARY DESTINATION bin
		CONFIGURATIONS All)
elseif(UNIX)
	install(TARGETS PreUtil
		LIBRARY DESTINATION bin
		CONFIGURATIONS All)
endif()

