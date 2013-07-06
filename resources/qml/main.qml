import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Particles 2.0
import QtQuick.Layouts 1.0

import Precursors.Networking 1.0

import Horde3D 1.0


Horde3DWindow {
	id: mainWindow

	width: 1024; height: 768

	color: "black"

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

	GridLayout {
		id: cameraControlsLayout
		columns: 2
		flow: GridLayout.LeftToRight
		anchors.top: parent.top
		anchors.left: parent.left

		Button {
			Layout.fillWidth: true
			text: "+"
			onClicked: {
				mainWindow.camera.zoom += 1;
			} // end onClicked
		} // end Button
		Button {
			Layout.fillWidth: true
			text: "-"
			onClicked: {
				mainWindow.camera.zoom -= 1;
			} // end onClicked
		} // end Button

		Button {
			Layout.fillWidth: true
			text: "<-"
			onClicked: {
				mainWindow.camera.yaw += 1;
			} // end onClicked
		} // end Button
		Button {
			Layout.fillWidth: true
			text: "->"
			onClicked: {
				mainWindow.camera.yaw -= 1;
			} // end onClicked
		} // end Button

		Button {
			Layout.fillWidth: true
			text: "/\\"
			onClicked: {
				mainWindow.camera.pitch += 1;
			} // end onClicked
		} // end Button
		Button {
			Layout.fillWidth: true
			text: "\\/"
			onClicked: {
				mainWindow.camera.pitch -= 1;
			} // end onClicked
		} // end Button
	} // end GridLayout

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
				model: ListModel { id: charList }
				delegate: GroupBox {
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
				highlight: Rectangle { color: "orange"; radius: 5 }
			}

			Button {
				Layout.fillWidth: true
				text: "Select Character"
			}
		}
	}
}
