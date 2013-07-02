import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import Precursors.Networking 1.0

ApplicationWindow {
	id: launcherWindow
	title: "RFI: Precursors Launcher"
	property int margin: 11
	property var mainWindow: mainWindowLoader.item;

	width: (mainLayout.implicitWidth + 2 * margin) + 80
    height: mainLayout.implicitHeight + 2 * margin

	minimumWidth: (mainLayout.Layout.minimumWidth + 2 * margin) + 20
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

        GroupBox {
            id: gridBox
            title: "Login"
            Layout.fillWidth: true

            GridLayout {
                id: gridLayout
                rows: 2
                flow: GridLayout.TopToBottom
                anchors.fill: parent

                Label { text: "Username" }
                Label { text: "Password" }

                TextField {
					id: username
                    Layout.fillWidth: true
				} // end Textfield

                TextField {
					id: password
					echoMode: TextInput.Password
                    Layout.fillWidth: true

					onAccepted: {
						loginBtn.login();
					} // end onAccepted
				} // end Textfield
            } // end GridLayout
        } // end Groupbox

        GroupBox {
            id: rowBox
            title: "Server"
            Layout.fillWidth: true

            RowLayout {
                id: rowLayout
				anchors.fill: parent

                TextField {
					id: server
					placeholderText: "localhost:6006"
                    Layout.fillWidth: true

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
			console.log("IT CONNECTED!");
		} // end onConnected

		onDisconnected: {
			mainWindow.hide();
			launcherWindow.show();
			console.log("Disconnected.");
		} // end onDisconnected
	} // end QChannels

	//-------------------------------------------------------------------------
} // end ApplicationWindow
