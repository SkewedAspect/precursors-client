# List project files
set(PLOGGING_HEADERS
	plogging.h
	plogging_global.h
   	${CMAKE_CURRENT_BINARY_DIR}/../config.h
	)
set(PLOGGING_SOURCES
	plogging.cpp
	)

add_library(PLogging SHARED
	${PLOGGING_HEADERS}
	${PLOGGING_SOURCES}
	)

set_target_properties(PLogging PROPERTIES COMPILE_FLAGS -fPIC)
set_target_properties(PLogging PROPERTIES DEBUG_POSTFIX _d)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}/..
	)

# Tell CMake what to link with
target_link_libraries(PLogging
	Qt5::Core
	)

# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command(TARGET PLogging POST_BUILD
		COMMAND copy \"$(TargetPath)"\"" .\\dist\\bin
		)
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX)
	set(ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dist/bin)
	set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR UNIX)


if(WIN32)
	install(TARGETS PLogging
		LIBRARY DESTINATION bin
		CONFIGURATIONS All)
elseif(UNIX)
	install(TARGETS PLogging
		LIBRARY DESTINATION bin
		CONFIGURATIONS All)
endif()

