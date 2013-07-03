import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Particles 2.0
import QtQuick.Layouts 1.0

import Precursors.Networking 1.0
import Ogre 1.0


ApplicationWindow {
	id: mainWindow

	width: 1024; height: 768

    color: "black"

	Component.onCompleted: {
		networking.connected.connect(onConnected);
	}

	function onConnected()
	{
		// Send a request for a list of characters.
		var charReq = networking.buildRequest("control", { type: "getCharacters" }, QChannels.CM_SECURE);
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

	/*
	OgreItem {
		id: ogreItem

		anchors.fill: parent


		Behavior on opacity { NumberAnimation { } }
		Behavior on width { NumberAnimation { } }
		Behavior on height { NumberAnimation { } }

		states: [
			State {
				name: "State1"

				PropertyChanges {
					target: ogreItem
					width: ogre.width
					height: ogre.height
				}
				PropertyChanges {
					target: toolbar1
					x: 5
					y: -toolbar1.height - 6
				}

				PropertyChanges {
					target: toolbar4
					anchors.top: ogreItem.top
					anchors.topMargin: 5
				}
				PropertyChanges {
					target: toolbar3
					anchors.top: ogreItem.top
					anchors.topMargin: 5
				}
				PropertyChanges {
					target: back
					opacity: 0
				}
			}
		]

		MouseArea {
			anchors.fill: parent
			acceptedButtons: Qt.LeftButton | Qt.RightButton

			property int prevX: -1
			property int prevY: -1

			onPositionChanged: {
				if (pressedButtons & Qt.LeftButton) {
					if (prevX > -1)
					ogreItem.camera.yaw -= (mouse.x - prevX) / 2
					if (prevY > -1)
					ogreItem.camera.pitch -= (mouse.y - prevY) / 2
					prevX = mouse.x
					prevY = mouse.y
				}
				if (pressedButtons & Qt.RightButton) {
					if (prevY > -1)
					ogreItem.camera.zoom = Math.min(6, Math.max(0.1, ogreItem.camera.zoom - (mouse.y - prevY) / 100));
					prevY = mouse.y
				}
			}
			onReleased: { prevX = -1; prevY = -1 }
		}
	}
	*/

	SubWindow {
		id: charWindow
		x: (parent.width - width) / 2
		y: (parent.height - height) / 2
		width: 300
		height: charWinLayout.implicitHeight + 4 * margin

		title: "Choose a Character"
		style: SubWindowStyle { }

		ColumnLayout {
			id: charWinLayout
			anchors.fill: parent
			anchors.margins: margin

			ListView {
				clip: true
				focus: true
				height: 80
				Layout.fillWidth: true
				model : ListModel { id: charList }
				delegate: GroupBox {
					anchors.left: parent.left
					anchors.right: parent.right

					Text {
						text: first_name + " " + middle_name + " " + last_name
						font.family: "Titillium Web";
						color: "white";
					}
				}
				highlight: Rectangle { color: "orange"; radius: 5 }
			}

			Button {
				Layout.fillWidth: true
				text: "Select Character"
			}
		}
	}
}
