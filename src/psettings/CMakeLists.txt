# List project files
set(PSETTINGS_HEADERS
	psettings.h
	psettings_global.h
   	${CMAKE_CURRENT_BINARY_DIR}/../config.h
	)
set(PSETTINGS_SOURCES
	psettings.cpp
	)

add_library(PSettings SHARED
	${PSETTINGS_HEADERS}
	${PSETTINGS_SOURCES}
	)

set_target_properties(PSettings PROPERTIES COMPILE_FLAGS -fPIC)
set_target_properties(PSettings PROPERTIES DEBUG_POSTFIX _d)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/..
	)

qt5_use_modules(PSettings
	Core
	)

# Tell CMake what to link with
target_link_libraries(PSettings
	#TODO: Commented until macports updates to CMake 2.8.11 or newer.
	#Qt5::Core

	PLogging
	)


# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command(TARGET PSettings POST_BUILD
		COMMAND copy \"$(TargetPath)"\"" .\\dist\\bin
		)
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX)
	set(ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dist/bin)
	set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR UNIX)


if(WIN32)
	install(TARGETS PSettings
		LIBRARY DESTINATION bin
		CONFIGURATIONS All)
elseif(UNIX)
	install(TARGETS PSettings
		LIBRARY DESTINATION bin
		CONFIGURATIONS All)
endif()

