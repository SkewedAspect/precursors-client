#!/bin/bash

case "$(uname)" in
	Darwin)
		SED_EXT_RE_OPT=-E
		;;
	Linux)
		SED_EXT_RE_OPT=-r
		;;
	*)
		echo "Unrecognized platform \"$(uname)\"!"
		exit 1
		;;
esac

function find_all_qmake5()
{
	# Returns as soon as it successfully finds a matching qmake executable.
	for qmakeName in qmake-qt5 qmake-5.1 qmake; do
		qmakePath="$(which $qmakeName 2>/dev/null)"

		if [ $? -eq 0 ]; then
			# Check Qt version
			"$qmakePath" -version | sed $SED_EXT_RE_OPT 's@^Using Qt version (5\.[0-9]+\.[0-9]+) in .*@\1:'"$qmakePath"'@p;d'
		fi
	done

	# Check paths relative to the current directory. (if you built Qt 5 from source in a similar directory to this one)
	for qmakePath in "$(dirname $0)/../qt5/qtbase/bin/qmake" "$(dirname $0)/../../other/qt5/qtbase/bin/qmake"; do
		if [ -x "$qmakePath" ]; then
			# Check Qt version
			"$qmakePath" -version | sed $SED_EXT_RE_OPT 's@^Using Qt version (5\.[0-9]+\.[0-9]+) in .*@\1:'"$qmakePath"'@p;d'
		fi
	done
}

if [ -z "$QMAKE" ]; then
	# Find all Qt 5.x versions of qmake, sort by Qt version (descending), and print the corresponding path.
	QMAKE=$(find_all_qmake5 | sort -nrt . -k 1,1 -k 2,2 -k 3,3 | head -n 1 | cut -d ':' -f 2)
fi

if [ ! -x "$QMAKE" ]; then
	echo -e "\033[1;31mA valid, executable 'qmake' was not found! (QMAKE=$QMAKE)\033[m"
	exit 1
fi
echo -e "\033[1;32mUsing 'qmake' at $QMAKE\033[m"

QT_INSTALL_BINS="$($QMAKE -query | awk -F : '$1 == "QT_INSTALL_BINS" {print $2}')"
QT_INSTALL_LIBS="$($QMAKE -query | awk -F : '$1 == "QT_INSTALL_LIBS" {print $2}')"

export PATH="$QT_INSTALL_BINS:$PATH"
export LD_LIBRARY_PATH="$QT_INSTALL_LIBS:$LD_LIBRARY_PATH"
export PKG_CONFIG_PATH="$QT_INSTALL_LIBS/pkgconfig:$PKG_CONFIG_PATH"

#cmake . -DCMAKE_MODULE_PATH:PATH="$QT_INSTALL_LIBS/cmake" $(for module in Core Gui Network OpenGL Qml Quick Widgets; do echo "-DQt5${module}_DIR:PATH=$QT_INSTALL_LIBS/cmake/Qt5Core"; done) "$@"
cmake . -DCMAKE_MODULE_PATH:PATH="$QT_INSTALL_LIBS/cmake" "$@"
