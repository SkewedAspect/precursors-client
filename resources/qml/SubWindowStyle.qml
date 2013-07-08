import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Private 1.0


Style {
    /*! The `SubWindow` attached to this style. */
    readonly property SubWindow control: __control

    /*! The margin. */
    property int margin: 6

    /*! The title text color. */
    property color textColor: "#ffffff"

    /*! The subwindow frame. */
    property Component panel: Rectangle {
        id: subPanel

        antialiasing: true
        smooth: true
        clip: true
        radius: 1

        property alias titleBar: titleBar
        property alias content: subContent

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#333" }
            GradientStop { position: 1.0; color: "#222" }
        }

        Text {
            id: titleText

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 4
            z: 1

            text: control.title
            color: textColor

            renderType: Text.NativeRendering
            font.weight: Font.DemiBold
            font.pointSize: 12
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Rectangle {
            id: titleBar

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: titleText.bottom
            anchors.topMargin: 1
            anchors.leftMargin: 1
            anchors.rightMargin: 1
            anchors.bottomMargin: -4

            smooth: true

            gradient: Gradient {
                GradientStop { position: 0.0; color: "#444444" }
                GradientStop { position: 0.33; color: "#3A3A3A" }
                GradientStop { position: 1.0; color: "#333333" }
            }

            Rectangle {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 1

                color: "#888888"
                opacity: 0.2
            }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 1
                height: 1

                color: "black"
                opacity: 0.5
            }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height: 1

                color: "#666666"
                opacity: 0.2
            }

            MouseArea {
                anchors.fill: parent
                drag.target: control
                drag.axis: "XandYAxis"
                drag.minimumX: 0
                drag.minimumY: 0
                drag.maximumX: control.parent.width - control.width
                drag.maximumY: control.parent.height - control.height
            }
        }

        Item {
            id: subContent

            anchors.top: titleBar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.topMargin: margin - 1
            anchors.leftMargin: margin
            anchors.rightMargin: margin
            anchors.bottomMargin: margin

            implicitWidth: control.implicitWidth + 2 * margin
            implicitHeight: control.implicitHeight + 2 * margin
        }

        Rectangle {
            anchors.fill: parent
            height: 1

            color: "transparent"

            border.width: 1
            border.color: "black"
            opacity: 0.35
        }
    }
}
