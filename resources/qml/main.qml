import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Particles 2.0
import QtQuick.Layouts 1.0

import Ogre 1.0


ApplicationWindow {
	id: mainWindow

	width: 1024; height: 768

    color: "black"

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

	SubWindow {
		x: 100; y: (parent.height - height) / 2
		width: 300; height: 80

		title: "Choose Your Destiny"

		ProgressBar {
			anchors.fill: parent
			value: 0.5
			style: progressBarStyle
		}

		style: SubWindowStyle { }
	}

	property Component progressBarStyle: ProgressBarStyle {
		background: BorderImage {
			source: "../progress-background.png"
			border.left: 2 ; border.right: 2 ; border.top: 2 ; border.bottom: 2
		}

		progress: Item {
			clip: true

			BorderImage {
				anchors.fill: parent
				anchors.rightMargin: (control.value < control.maximumValue) ? -4 : 0
				source: "../progress-fill.png"
				border.left: 2 ; border.right: 2 ; border.top: 2 ; border.bottom: 2

				Rectangle {
					width: 1
					color: "#a70"
					opacity: 0.8
					anchors.top: parent.top
					anchors.bottom: parent.bottom
					anchors.bottomMargin: 1
					anchors.right: parent.right
					visible: control.value < control.maximumValue
					anchors.rightMargin: -parent.anchors.rightMargin
				}
			}

			ParticleSystem { id: bubbles }
			ImageParticle {
				id: fireball
				system: bubbles
				source: "../bubble.png"
				opacity: 0.7
			}
			Emitter {
				system: bubbles
				anchors.bottom: parent.bottom
				anchors.margins: 4
				anchors.bottomMargin: -4
				anchors.left: parent.left
				anchors.right: parent.right
				size: 4
				sizeVariation: 4
				acceleration: PointDirection{ y: -6; xVariation: 3 }
				emitRate: 6 * control.value
				lifeSpan: 3000
			}
		}
	}
}
