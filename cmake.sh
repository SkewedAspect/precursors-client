#!/bin/bash

if [ -z "$QMAKE" ]; then
	QMAKE=$PWD/../qt5/qtbase/bin/qmake
fi
QT_INSTALL_BINS="$($QMAKE -query | awk -F : '$1 == "QT_INSTALL_BINS" {print $2}')"
QT_INSTALL_LIBS="$($QMAKE -query | awk -F : '$1 == "QT_INSTALL_LIBS" {print $2}')"

export PATH="$QT_INSTALL_BINS:$PATH"
export LD_LIBRARY_PATH="$QT_INSTALL_LIBS:$LD_LIBRARY_PATH"
export PKG_CONFIG_PATH="$QT_INSTALL_LIBS/pkgconfig:$PKG_CONFIG_PATH"

#cmake . -DCMAKE_MODULE_PATH:PATH="$QT_INSTALL_LIBS/cmake" $(for module in Core Gui Network OpenGL Qml Quick Widgets; do echo "-DQt5${module}_DIR:PATH=$QT_INSTALL_LIBS/cmake/Qt5Core"; done) "$@"
cmake . -DCMAKE_MODULE_PATH:PATH="$QT_INSTALL_LIBS/cmake" "$@"
