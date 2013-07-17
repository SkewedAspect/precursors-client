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

	devices/joystickdevice.h
	devices/inputdevice.h
	devices/mousedevice.h
	devices/keyboarddevice.h

	slots/digitalcontrolslot.h
	slots/controlslot.h
	slots/analogcontrolslot.h

	ois/oisdriver.h

	qt/qtdriver.h

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

	devices/joystickdevice.cpp
	devices/inputdevice.cpp
	devices/mousedevice.cpp
	devices/keyboarddevice.cpp

	slots/digitalcontrolslot.cpp
	slots/controlslot.cpp
	slots/analogcontrolslot.cpp

	ois/oisdriver.cpp

	qt/qtdriver.cpp
	)

add_library(Controls SHARED
	${CONTROLS_HEADERS}
	${CONTROLS_SOURCES}
	)

set_target_properties(Controls PROPERTIES COMPILE_FLAGS -fPIC)
set_target_properties(Controls PROPERTIES DEBUG_POSTFIX _d)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}/..
	)

qt5_use_modules(Controls
	Gui
	Qml
	Quick
	Core
	)

# Tell CMake what to link with
#TODO: Commented until macports updates to CMake 2.8.11 or newer.
target_link_libraries(Controls
	#Qt5::Gui
	#Qt5::Qml
	#Qt5::Quick
	#Qt5::Core

	OIS
	)


# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command(TARGET Controls POST_BUILD
		COMMAND copy \"$(TargetPath)"\"" .\\dist\\bin
		)
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX)
	set(ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dist/bin)
	set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR UNIX)


if(WIN32)
	install(TARGETS Controls
		LIBRARY DESTINATION bin
		CONFIGURATIONS All)
elseif(UNIX)
	install(TARGETS Controls
		LIBRARY DESTINATION bin
		CONFIGURATIONS All)
endif()

