import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.1

import Precursors.Networking 1.0


GameWindow {
    id: mainWindow

    SubWindow {
        id: chatWindow
        x: 10
        y: 10
        width: 450
        height: 300
        opacity: .2
        title: "Insert Chat Here"
        style: SubWindowStyle {}

        Component.onCompleted: {
            networking.incomingMessage.connect(function(channel, event){
                if (channel == 'chat')
                {
                    if (event.action == 'message')
                    {
                        chatText.text +=  event.user + ': ' + event.payload + '\n';
                        chatText.cursorPosition = chatText.text.length

                    }
                    else if (event.action == 'notification')
                    {
                        chatText.test += '\n' + event.payload;
                    }
                }
            });

        }

        Behavior on opacity {
            NumberAnimation{
                duration: 100
            }
        }

        MouseArea {
            id: chatArea
            hoverEnabled: true
            anchors.fill: parent
            onEntered: {
                chatWindow.opacity = .8;
            }
            onExited: {
                chatWindow.opacity = .2;
            }
            GridLayout {
                id: chatGrid
                rows: 2
                columns: 2
                anchors.fill: parent
                Layout.fillHeight: true

                TextArea {
                    id: chatText
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    textColor: "white"
                    Layout.columnSpan: 2
                    Layout.row: 0
                    backgroundVisible: false
                    readOnly: true
                    wrapMode: TextEdit.Wrap

                    style: TextAreaStyle {
                        textColor: "#00000000"
                    }
                }

                TextField {
                    id: chatField
                    Layout.row: 1
                    Layout.column: 0
                    Layout.alignment: Qt.AlignBottom
                    Layout.fillWidth: true
                    focus: false

                    Keys.onEscapePressed: {
                        focus = false;
                    }

                    onAccepted: {
                        chatButton.sendMessage();
                    }
                }

                Button {
                    id: chatButton
                    text: "Send"
                    tooltip: "Press to send message"
                    Layout.row: 1
                    Layout.column: 1
                    Layout.alignment: Qt.AlignBottom

                    // TODO: implement commands, shortcuts
                    onClicked: {
                        sendMessage();
                    }

                    property var sendMessage: function()
                    {
                        networking.sendEvent("chat", {'room': 'global', 'action': 'message', 'message': chatField.text}, PChannels.CM_RELIABLE)
                        chatField.text = '';
                    }

                    Keys.onEnterPressed: {
                        console.log('Enter pressed')
                        sendMessage();
                    }

                    Keys.onReturnPressed: {
                        sendMessage();
                    }
                }
            }
        }
    }

    DebugWindow {
        id: debugWindow

        x: parent.width - width
        y: parent.height - height

        angularTgtVelMax: Math.PI
        //angularTgtVelMax: 0.7853981633974483
    }

    SubWindow {
        id: charWindow
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 300
        height: charWinLayout.implicitHeight + 4 * margin
        opacity: .9

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
						   return name;
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

                            controls.currentContext = controls.context('flightsim');
                        } // end if
                    } // end onCharSelReply
                }
            }
        }
    }
}
