import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.0

import Precursors.Networking 1.0
import Precursors.Settings 1.0

ApplicationWindow {
	id: launcherWindow
	title: "RFI: Precursors Launcher"

	property int margin: 11
	property var mainWindow: mainWindowLoader.item;

	width: (mainLayout.implicitWidth + 2 * margin) + 80
    height: mainLayout.implicitHeight + 2 * margin

	minimumWidth: (mainLayout.Layout.minimumWidth + 2 * margin) + 80
    minimumHeight: mainLayout.Layout.minimumHeight + 2 * margin

	maximumHeight: minimumHeight
	maximumWidth: (mainLayout.implicitWidth + 2 * margin) + 80

	// Make the window show itself.
    Component.onCompleted: launcherWindow.show();

	Loader {
		id: mainWindowLoader
		source: "main.qml"
	} // end Loader

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: margin

		Text {
			id: title
			anchors.fill: parent
			text: "RFI: Precursors"
			horizontalAlignment: Text.AlignHCenter

			font.family: "Trajan Pro";
			font.pointSize: 42;
		} // end Text

        GroupBox {
            id: gridBox
            Layout.fillWidth: true

            GridLayout {
                id: gridLayout
                rows: 2
                flow: GridLayout.TopToBottom
                anchors.fill: parent

				Label {
					text: "Username"
					font.family: "Titillium Web";
				} // end Label

				Label {
					text: "Password"
					font.family: "Titillium Web";
				} // end Label

                TextField {
					id: username
					focus: true
                    Layout.fillWidth: true
					font.family: "Titillium Web";

					text: settingsMan.get('lastUser', "");
				} // end Textfield

                TextField {
					id: password
					echoMode: TextInput.Password
                    Layout.fillWidth: true
					font.family: "Titillium Web";

					onAccepted: {
						loginBtn.login();
					} // end onAccepted
				} // end Textfield
            } // end GridLayout
        } // end Groupbox

        GroupBox {
            id: rowBox
            Layout.fillWidth: true

            RowLayout {
                id: rowLayout
				anchors.fill: parent

                TextField {
					id: server
					placeholderText: "localhost:6006"
                    Layout.fillWidth: true
					font.family: "Titillium Web";

					text: {
						var serverText = settingsMan.get("server", "localhost") + ":" + settingsMan.get("port", "6006");
						if(serverText != "localhost:6006")
						{
							return serverText;
						} // end if

						return "";
					}

					onAccepted: {
						loginBtn.login();
					} // end onAccepted
				} // end TextField

                Button {
					id: loginBtn
                    text: "Login"
					isDefault: true

					property var login: function()
					{
						var parts = server.text.split(':');
						var svrAddress = parts[0] || "localhost";
						var svrPort = parseInt(parts[1]) || 6006;

						settingsMan.set('server', svrAddress);
						settingsMan.set('port', svrPort);

						settingsMan.save();

						// Connect to the server
						networking.connectToServer(svrAddress, svrPort, username.text, password.text);
					} // end login

					onClicked: {
						login();
					} // end onClicked
                } // end Button
            } // end RowLayout
        } // end GroupBox
    } // end ColumnLayout

	//-------------------------------------------------------------------------
	// The Networking stack
	//-------------------------------------------------------------------------

	QChannels {
		id: networking

		onConnected: {
			mainWindow.show();
			launcherWindow.hide();
			settingsMan.set('lastUser', username.text);
			settingsMan.save();
		} // end onConnected

		onDisconnected: {
			mainWindow.hide();
			launcherWindow.show();
			console.log("Disconnected.");
		} // end onDisconnected
	} // end QChannels

	//-------------------------------------------------------------------------
} // end ApplicationWindow
