import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Particles 2.0
import QtQuick.Layouts 1.0

import Precursors.Networking 1.0

import Horde3D 1.0

import "../js/logging.js" as Logging


Horde3DWindow {
	id: mainWindow

	width: 1024; height: 768
	color: "black"

	property var logger: new Logging.Logger("mainWindow")

	property var headingSpeed: 1
	property var pitchSpeed: 1

	property var headingVel: 0
	property var pitchVel: 0

	Component.onCompleted: {
		networking.connected.connect(onConnected);
	}

	function onConnected()
	{
		// Send a request for a list of characters.
		var charReq = networking.buildRequest("control", { type: "getCharacters" }, PChannels.CM_SECURE);
		charReq.reply.connect(onCharListReply);
		charReq.send();

		function onCharListReply(confirmed)
		{
			var characters = charReq.replyMessage.characters;

			charList.clear();
			characters.forEach(function(character)
			{
				charList.append(character);
			}); // end forEach
		} // end onCharListReply
	} // end onConnected

	function onFPSChanged() {
		onFPSChanged.fpsWindow = onFPSChanged.fpsWindow || [];
		onFPSChanged.fpsSum = onFPSChanged.fpsSum || 0;

		onFPSChanged.fpsSum += mainWindow.fps;
		onFPSChanged.fpsWindow.push(mainWindow.fps);

		if(onFPSChanged.fpsWindow.length > 100)
		{
			onFPSChanged.fpsSum -= onFPSChanged.fpsWindow.shift();
		}

		var fps = (onFPSChanged.fpsSum / onFPSChanged.fpsWindow.length).toFixed(1);
		while(fps.length < 6)
		{
			fps = ' ' + fps;
		}
		fpsText.text = "FPS: " + fps;
	} // end onFPSChanged
	onFpsChanged: onFPSChanged()

	FocusScope {
		focus: true
		anchors.fill: parent

		Timer {
			id: rotateTimer
			interval: 16
			running: false
			repeat: true
			onTriggered: {
				if(headingVel == 0 && pitchVel == 0)
				{
					rotateTimer.stop();
				}
				else
				{
					horde3d.avatar.heading += headingVel;
					horde3d.avatar.pitch += pitchVel;
				}
			}
		}

		function doKeyShit(event, release) {
			var factor = 1;
			if(release)
			{
				factor = -1;
			}

			// On OSX, Qt 5.1 has a bug where event.isAutoRepeat isn't correctly set, and it segfaults when you check
			// it too much.
			if(Qt.platform != "osx" && event.isAutoRepeat)
			{
				factor = 0;
			}

			switch(event.key) {
				case Qt.Key_MediaTogglePlayPause:
					if(Qt.platform == "osx")
					{
						pitchVel = 0;
						headingVel = 0;
					}
					break;
				case Qt.Key_F8:
					if(Qt.platform != "osx")
					{
						pitchVel = 0;
						headingVel = 0;
					}
					break;

				case Qt.Key_Up:
					pitchVel += factor * pitchSpeed;
					break;
				case Qt.Key_Down:
					pitchVel -= factor * pitchSpeed;
					break;

				case Qt.Key_Left:
					headingVel -= factor * headingSpeed;
					break;
				case Qt.Key_Right:
					headingVel += factor * headingSpeed;
					break;

				default:
					return;
			}

			rotateTimer.start();
			event.accepted = true;
		}

		Keys.onPressed: doKeyShit(event, false)
		Keys.onReleased: doKeyShit(event, true)

		MouseArea {
			id: wrapper
			anchors.fill: parent

			NumberAnimation {
				id: animateRotation
				properties: "pitch"
				from: 0.0
				to: 360.0
				loops: Animation.Infinite
				duration: 1000
			}

			function updateRotation(mouse) {
				var mouseSensitivity = settings.get('mouseSensitivity', 0.1);
				if(updateRotation.lastX !== undefined && updateRotation.lastY !== undefined)
				{
					mainWindow.camDolly.heading -= (mouse.x - (updateRotation.lastX || mouse.x)) * mouseSensitivity;
					mainWindow.camDolly.pitch -= (mouse.y - (updateRotation.lastY || mouse.y)) * mouseSensitivity;
					//horde3d.avatar.heading -= (mouse.x - (updateRotation.lastX || mouse.x)) * mouseSensitivity;
					//horde3d.avatar.pitch -= (mouse.y - (updateRotation.lastY || mouse.y)) * mouseSensitivity;
				} // end if

				updateRotation.lastX = mouse.x;
				updateRotation.lastY = mouse.y;
			}

			onPressed: {
				updateRotation.lastX = mouse.x;
				updateRotation.lastY = mouse.y;
			}
			onPositionChanged: {
				updateRotation(mouse);
			}
			/*
			onClicked: {
				animateRotation.target = horde3d.avatar;
				animateRotation.start();
			}
			*/
		}

		Text {
			id: fpsText
			anchors.top: parent.top
			anchors.right: parent.right

			color: "white"
			font.family: "monospace"
			font.pointSize: 24
			font.weight: Font.DemiBold
		}

		SubWindow {
			id: charWindow
			//x: (parent.width - width) / 2
			x: 100
			y: (parent.height - height) / 2
			width: 300
			height: charWinLayout.implicitHeight + 4 * margin

			title: "Choose a Character"
			style: SubWindowStyle { }

			Component {
				id: characterItem

				MouseArea {
					id: wrapper
					anchors.left: parent.left
					anchors.right: parent.right
					//anchors.margins: 4
					height: group.height

					onClicked: {
						wrapper.ListView.view.currentIndex = index;
					}

					GroupBox {
						id: group
						anchors.left: parent.left
						anchors.right: parent.right

						Text {
							text: {
								try
								{
									return first_name + " " + middle_name + " " + last_name;
								}
								catch(e)
								{
									return first_name + " " + last_name;
								}
							}
							font.family: "Titillium Web";
							color: "white";
						}
					}
				}
			}

			ColumnLayout {
				id: charWinLayout
				anchors.fill: parent
				anchors.margins: margin

				ListView {
					clip: true
					height: 80
					Layout.fillWidth: true

					model: ListModel { id: charList }
					delegate: characterItem
					highlight: Rectangle { color: "orange"; radius: 5 }
				}

				Button {
					Layout.fillWidth: true
					text: "Select Character"
				}
			}
		}
	}
}
