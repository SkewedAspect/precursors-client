import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.0

import Precursors.Networking 1.0


GameWindow {
    id: mainWindow

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
					wrapper.ListView.view.currentItemID = charID;
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
				id: charListView
				clip: true
				height: 80
				Layout.fillWidth: true

				property var currentItemID

				model: ListModel { id: charList }
				delegate: characterItem
				highlight: Rectangle { color: "orange"; radius: 5 }
			}

			Button {
				Layout.fillWidth: true
				text: "Select Character"
				onClicked: {
					// Log in to the currently selected character.
					var charReq = networking.buildRequest("control",
							{ type: "selectCharacter", character: charListView.currentItemID },
							PChannels.CM_SECURE);
					charReq.reply.connect(onCharSelReply);
					charReq.send();

					function onCharSelReply(confirmed)
					{
						if(confirmed)
						{
							charWindow.visible = false;
						} // end if
					} // end onCharSelReply
				}
			}
        }
    }
}
