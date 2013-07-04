import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Particles 2.0
import QtQuick.Layouts 1.0

import Horde3D 1.0


Horde3DWindow {
	id: mainWindow

    Component.onCompleted: mainWindow.show();

	width: 1024; height: 768

    color: "black"

	/*
	SubWindow {
		x: 600; y: (parent.height - height) / 2
		width: 800; height: 600

		title: "HOLY CRAP HORDE"

		style: SubWindowStyle { }

		Horde3DItem {
			id: h3dItem

			anchors.fill: parent


			Behavior on opacity { NumberAnimation { } }
			Behavior on width { NumberAnimation { } }
			Behavior on height { NumberAnimation { } }

			states: [
				State {
					name: "State1"

					PropertyChanges {
						target: h3dItem
						width: h3d.width
						height: h3d.height
					}
					PropertyChanges {
						target: toolbar1
						x: 5
						y: -toolbar1.height - 6
					}

					PropertyChanges {
						target: toolbar4
						anchors.top: h3dItem.top
						anchors.topMargin: 5
					}
					PropertyChanges {
						target: toolbar3
						anchors.top: h3dItem.top
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
						h3dItem.camera.yaw -= (mouse.x - prevX) / 2
						if (prevY > -1)
						h3dItem.camera.pitch -= (mouse.y - prevY) / 2
						prevX = mouse.x
						prevY = mouse.y
					}
					if (pressedButtons & Qt.RightButton) {
						if (prevY > -1)
						h3dItem.camera.zoom = Math.min(6, Math.max(0.1, h3dItem.camera.zoom - (mouse.y - prevY) / 100));
						prevY = mouse.y
					}
				}
				onReleased: { prevX = -1; prevY = -1 }
			}
		}
	}
	*/

	/*
	ParticleSystem { id: stars2 }
	ImageParticle {
		id: star2
		system: stars2

		source: "./bubble.png"
		opacity: 0.7
	}
	Emitter {
		id: starEmitter2
		system: stars2

		anchors.top: parent.top
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		width: 1

		size: 1
		sizeVariation: 1
		lifeSpan: 3000

		velocity: PointDirection { x: -1000; xVariation: 500 }
		emitRate: 80
	}
	*/

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
			source: "./progress-background.png"
			border.left: 2 ; border.right: 2 ; border.top: 2 ; border.bottom: 2
		}

		progress: Item {
			clip: true

			BorderImage {
				anchors.fill: parent
				anchors.rightMargin: (control.value < control.maximumValue) ? -4 : 0
				source: "./progress-fill.png"
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

			ParticleSystem { id: stars }
			ImageParticle {
				id: star
				system: stars

				source: "./bubble.png"
				opacity: 0.7
			}
			Emitter {
				id: starEmitter
				system: stars

				anchors.top: parent.top
				anchors.right: parent.right
				anchors.bottom: parent.bottom
				anchors.margins: 2
				width: 1

				size: 1
				sizeVariation: 1
				lifeSpan: 3000

				velocity: PointDirection { x: -100; xVariation: 50 }
				emitRate: 20 * control.value
			}

			MouseArea {
				anchors.fill: parent
				/*
				onClicked: {
					starEmitter.burst(100)
				}
				*/
				onPressed: {
					laserEmitter.enabled = true
				}
				onReleased: {
					laserEmitter.enabled = false
				}
				onPositionChanged: {
					laserEmitter.x = mouse.x
					laserEmitter.y = mouse.y
				}
			}

			ParticleSystem { id: lasers }
			/*
			ImageParticle {
				id: laser
				system: lasers

				source: "./progress-background.png"
				opacity: 0.7
			}
			*/
			ItemParticle {
				system: lasers
				delegate: Rectangle {
					color: "red"
					width: 10; height: 1
				}
			}
			Emitter {
				id: laserEmitter
				system: lasers
				enabled: false

				width: 1
				height: 1

				size: 5
				lifeSpan: 1000

				velocity: PointDirection { x: -1000 }
				emitRate: 200
			}

			/*
			ParticleSystem { id: bubbles }
			ImageParticle {
				id: fireball
				system: bubbles
				source: "./bubble.png"
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
			*/
		}
	}
}
