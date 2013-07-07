# List project files
set(CLIENT_HEADERS
	cameranodeobject.h
	entity.h
	h3ditem.h
	h3ditemanim.h
	horde3dwindow.h
	math3d.h
   	${CMAKE_CURRENT_BINARY_DIR}/../config.h
	)
set(CLIENT_SOURCES
	main.cpp
	cameranodeobject.cpp
	entity.cpp
	h3ditem.cpp
	h3ditemanim.cpp
	horde3dwindow.cpp
	math3d.cpp
	)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}/..
	${Horde3D_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
	)

add_executable(PrecursorsClient WIN32
	${CLIENT_HEADERS}
	${CLIENT_SOURCES}
	)

set_target_properties(PrecursorsClient PROPERTIES COMPILE_FLAGS -fPIC)
set_target_properties(PrecursorsClient PROPERTIES DEBUG_POSTFIX _d)

# Tell CMake what to link with
target_link_libraries(PrecursorsClient
	${Horde3D_LIBRARIES}

	Qt5::Core
	Qt5::Gui
	Qt5::Qml
	Qt5::Quick
	Qt5::OpenGL

	${OPENGL_gl_LIBRARY}

	python2.7

	PLogging
	PreUtil
	PChannels
	PSettings
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
