# List project files
set(CLIENT_HEADERS
	cameranodeobject.h
	entity.h
	horde3ditem.h
	horde3ditemanim.h
	horde3dmanager.h
	horde3dwindow.h
	math3d.h
	timestamp_sync.h
	${CMAKE_CURRENT_BINARY_DIR}/../config.h
	)
set(CLIENT_SOURCES
	main.cpp
	cameranodeobject.cpp
	entity.cpp
	horde3ditem.cpp
	horde3ditemanim.cpp
	horde3dmanager.cpp
	horde3dwindow.cpp
	math3d.cpp
	timestamp_sync.cpp
	)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${Horde3D_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
	)

add_executable(PrecursorsClient WIN32
	${CLIENT_HEADERS}
	${CLIENT_SOURCES}
	)

set_target_properties(PrecursorsClient PROPERTIES COMPILE_FLAGS -fPIC)
set_target_properties(PrecursorsClient PROPERTIES DEBUG_POSTFIX _d)

qt5_use_modules(PrecursorsClient
	Core
	Gui
	Qml
	Quick
	OpenGL
	)

# Tell CMake what to link with
target_link_libraries(PrecursorsClient
	${Horde3D_LIBRARIES}

	#TODO: Commented until macports updates to CMake 2.8.11 or newer.
	#Qt5::Core
	#Qt5::Gui
	#Qt5::Qml
	#Qt5::Quick
	#Qt5::OpenGL

	${OPENGL_gl_LIBRARY}

	python2.7

	PLogging
	PreUtil
	PChannels
	PSettings
	Controls
	)


# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command(TARGET PrecursorsClient POST_BUILD
		COMMAND copy \"$(TargetPath)"\"" .\\dist\\bin
		)
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR UNIX)


if(WIN32)
	install(TARGETS PrecursorsClient
		RUNTIME DESTINATION bin
		CONFIGURATIONS All)

	install(DIRECTORY ${CMAKE_SOURCE_DIR}/dist/Media
		DESTINATION ./
		CONFIGURATIONS Release RelWithDebInfo Debug
		)

	install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins.cfg
		${CMAKE_SOURCE_DIR}/dist/bin/resources.cfg
		DESTINATION bin
		CONFIGURATIONS Release RelWithDebInfo
		)

	install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins_d.cfg
		${CMAKE_SOURCE_DIR}/dist/bin/resources_d.cfg
		DESTINATION bin
		CONFIGURATIONS Debug
		)

elseif(UNIX)
	install(TARGETS PrecursorsClient
		RUNTIME DESTINATION bin
		CONFIGURATIONS All)

	install(DIRECTORY ${CMAKE_SOURCE_DIR}/dist/media
		DESTINATION ./
		CONFIGURATIONS Release RelWithDebInfo Debug
		)

	install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins.cfg
		${CMAKE_SOURCE_DIR}/dist/bin/resources.cfg
		DESTINATION bin
		CONFIGURATIONS Release RelWithDebInfo Debug
		)

endif()
