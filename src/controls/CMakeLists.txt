# List project files
set(CONTROLS_HEADERS
	controlbindingmap.h
	controlcontext.h
	controlsmanager.h
	controls_global.h
	inputdriver.h

	bindings/controlbinding.h
	bindings/buttondigitalbinding.h
	bindings/buttonanalogbinding.h
	bindings/axisanalogbinding.h
	bindings/axisdigitalbinding.h

	signals/axisinputsignal.h
	signals/inputsignal.h
	signals/buttoninputsignal.h

	devices/inputdevice.h
	devices/genericdevice.h

	slots/digitalcontrolslot.h
	slots/controlslot.h
	slots/analogcontrolslot.h

   	${CMAKE_CURRENT_BINARY_DIR}/../config.h
	)

set(CONTROLS_SOURCES
	controlbindingmap.cpp
	controlcontext.cpp
	controlsmanager.cpp
	inputdriver.cpp

	bindings/controlbinding.cpp
	bindings/buttondigitalbinding.cpp
	bindings/buttonanalogbinding.cpp
	bindings/axisanalogbinding.cpp
	bindings/axisdigitalbinding.cpp

	signals/axisinputsignal.cpp
	signals/inputsignal.cpp
	signals/buttoninputsignal.cpp

	devices/inputdevice.cpp
	devices/genericdevice.cpp

	slots/digitalcontrolslot.cpp
	slots/controlslot.cpp
	slots/analogcontrolslot.cpp
	)

add_library(Controls SHARED
	${CONTROLS_HEADERS}
	${CONTROLS_SOURCES}
	)

set_target_properties(Controls PROPERTIES COMPILE_FLAGS -fPIC)
set_target_properties(Controls PROPERTIES DEBUG_POSTFIX _d)

qt5_use_modules(Controls
	Gui
	Qml
	Quick
	Core
	)

# Tell CMake what to link with
target_link_libraries(Controls
	#TODO: Commented until macports updates to CMake 2.8.11 or newer.
	#Qt5::Gui
	#Qt5::Qml
	#Qt5::Quick
	#Qt5::Core

	PLogging
	PSettings
	)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${OIS_INCLUDE_DIR}
	)


#### OIS Driver ####

set(CONTROLS_OISDRIVER_HEADERS
	ois/oisdriver.h
	)

set(CONTROLS_OISDRIVER_SOURCES
	ois/oisdriver.cpp
	)

add_library(ControlsOISDriver SHARED
	${CONTROLS_OISDRIVER_HEADERS}
	${CONTROLS_OISDRIVER_SOURCES}
	)

set_target_properties(ControlsOISDriver PROPERTIES COMPILE_FLAGS -fPIC)
set_target_properties(ControlsOISDriver PROPERTIES DEBUG_POSTFIX _d)

qt5_use_modules(ControlsOISDriver
	Gui
	Core
	)

target_link_libraries(ControlsOISDriver
	#TODO: Commented until macports updates to CMake 2.8.11 or newer.
	#Qt5::Gui
	#Qt5::Core

	OIS

	Controls
	PLogging
	PSettings
	)


#### Qt Driver ####

set(CONTROLS_QTDRIVER_HEADERS
	qt/qtdriver.h
	)

set(CONTROLS_QTDRIVER_SOURCES
	qt/qtdriver.cpp
	)

add_library(ControlsQtDriver SHARED
	${CONTROLS_QTDRIVER_HEADERS}
	${CONTROLS_QTDRIVER_SOURCES}
	)

set_target_properties(ControlsQtDriver PROPERTIES COMPILE_FLAGS -fPIC)
set_target_properties(ControlsQtDriver PROPERTIES DEBUG_POSTFIX _d)

qt5_use_modules(ControlsQtDriver
	Gui
	Qml
	Quick
	Core
	)

target_link_libraries(ControlsQtDriver
	#TODO: Commented until macports updates to CMake 2.8.11 or newer.
	#Qt5::Gui
	#Qt5::Qml
	#Qt5::Quick
	#Qt5::Core

	Controls
	PLogging
	PSettings
	)


# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command(TARGET Controls POST_BUILD
		COMMAND copy \"$(TargetPath)"\"" .\\dist\\bin
		)
	add_custom_command(TARGET ControlsOISDriver POST_BUILD
		COMMAND copy \"$(TargetPath)"\"" .\\dist\\bin
		)
	add_custom_command(TARGET ControlsQtDriver POST_BUILD
		COMMAND copy \"$(TargetPath)"\"" .\\dist\\bin
		)
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX)
	set_target_properties(ControlsOISDriver PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dist/bin)
	set_target_properties(ControlsOISDriver PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dist/bin)

	set_target_properties(ControlsQtDriver PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dist/bin)
	set_target_properties(ControlsQtDriver PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR UNIX)


if(WIN32 OR UNIX)
	install(TARGETS Controls LIBRARY DESTINATION bin CONFIGURATIONS All)
	install(TARGETS ControlsOISDriver LIBRARY DESTINATION bin CONFIGURATIONS All)
	install(TARGETS ControlsQtDriver LIBRARY DESTINATION bin CONFIGURATIONS All)
endif()
