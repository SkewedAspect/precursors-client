import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.0

import Precursors.Networking 1.0


SubWindow {
	id: debugWindow
	anchors.top: parent.top
	anchors.right: parent.right
	height: debugWindowLayout.implicitHeight + 4 * margin
	width: debugWindowLayout.implicitWidth + 4 * margin

	property real angularTgtVelMax: Math.PI

	property alias posTgtVelHeading: posTgtVelHeading
	property alias negTgtVelHeading: negTgtVelHeading
	property alias posCamHeading: posCamHeading
	property alias negCamHeading: negCamHeading
	property alias posShipHeading: posShipHeading
	property alias negShipHeading: negShipHeading
	property alias posTgtVelPitch: posTgtVelPitch
	property alias negTgtVelPitch: negTgtVelPitch
	property alias posCamPitch: posCamPitch
	property alias negCamPitch: negCamPitch
	property alias posShipPitch: posShipPitch
	property alias negShipPitch: negShipPitch
	property alias posTgtVelRoll: posTgtVelRoll
	property alias negTgtVelRoll: negTgtVelRoll
	property alias posCamRoll: posCamRoll
	property alias negCamRoll: negCamRoll
	property alias posShipRoll: posShipRoll
	property alias negShipRoll: negShipRoll

	GridLayout {
		id: debugWindowLayout
		anchors.fill: parent
		anchors.margins: margin
		flow: GridLayout.TopToBottom
		rows: 3

		//////////////////////////////////
		Text {
			text: "heading"
			color: "white"
			Layout.columnSpan: 3
		}

		Loader {
			id: posTgtVelHeading
			sourceComponent: tgtVelProgressBar
		}
		Loader {
			id: negTgtVelHeading
			sourceComponent: tgtVelProgressBar
			rotation: 180
		}

		Loader {
			id: posCamHeading
			sourceComponent: camProgressBar
		}
		Loader {
			id: negCamHeading
			sourceComponent: camProgressBar
			rotation: 180
		}

		Loader {
			id: posShipHeading
			sourceComponent: shipProgressBar
		}
		Loader {
			id: negShipHeading
			sourceComponent: shipProgressBar
			rotation: 180
		}

		//////////////////////////////////
		Text {
			text: "pitch"
			color: "white"
			Layout.columnSpan: 3
		}

		Loader {
			id: posTgtVelPitch
			sourceComponent: tgtVelProgressBar
		}
		Loader {
			id: negTgtVelPitch
			sourceComponent: tgtVelProgressBar
			rotation: 180
		}

		Loader {
			id: posCamPitch
			sourceComponent: camProgressBar
		}
		Loader {
			id: negCamPitch
			sourceComponent: camProgressBar
			rotation: 180
		}

		Loader {
			id: posShipPitch
			sourceComponent: shipProgressBar
		}
		Loader {
			id: negShipPitch
			sourceComponent: shipProgressBar
			rotation: 180
		}

		//////////////////////////////////
		Text {
			text: "roll"
			color: "white"
			Layout.columnSpan: 3
		}

		Loader {
			id: posTgtVelRoll
			sourceComponent: tgtVelProgressBar
		}
		Loader {
			id: negTgtVelRoll
			sourceComponent: tgtVelProgressBar
			rotation: 180
		}

		Loader {
			id: posCamRoll
			sourceComponent: camProgressBar
		}
		Loader {
			id: negCamRoll
			sourceComponent: camProgressBar
			rotation: 180
		}

		Loader {
			id: posShipRoll
			sourceComponent: shipProgressBar
		}
		Loader {
			id: negShipRoll
			sourceComponent: shipProgressBar
			rotation: 180
		}
	}


	Component {
		id: pbStyleBackground

		Rectangle {
			id: background

			implicitWidth: 200
			implicitHeight: 16
			color: "#777777"
			border.color: "#555555"
		}
	}

	Component {
		id: tgtVelProgressBar

		Item {
			implicitWidth: pbar.implicitWidth
			implicitHeight: pbar.implicitHeight

			property real progress: 0

			ProgressBar {
				id: pbar
				maximumValue: angularTgtVelMax
				minimumValue: 0
				value: progress
				orientation: Qt.Vertical
				anchors.fill: parent

				style: ProgressBarStyle {
					background: Loader { sourceComponent: pbStyleBackground }

					progress: Rectangle {
						color: "#00c"
						border.color: "#44000088"

						Rectangle {
							color: "transparent"
							border.color: "#440000ff"
							anchors.fill: parent
							anchors.margins: 1
						}
					}
				}
			}

			Text {
				color: "white"
				text: "target velocity"
				anchors.top: parent.top
				anchors.topMargin: 3
				anchors.left: parent.right
				transformOrigin: Item.TopLeft
				rotation: 90
			}
		}
	}

	Component {
		id: camProgressBar

		Item {
			implicitWidth: pbar.implicitWidth
			implicitHeight: pbar.implicitHeight

			property real progress: 0

			ProgressBar {
				id: pbar
				maximumValue: Math.PI
				minimumValue: 0
				value: progress
				orientation: Qt.Vertical
				anchors.fill: parent

				style: ProgressBarStyle {
					background: Loader { sourceComponent: pbStyleBackground }

					progress: Rectangle {
						color: "#c00"
						border.color: "#44880000"

						Rectangle {
							color: "transparent"
							border.color: "#44ff0000"
							anchors.fill: parent
							anchors.margins: 1
						}
					}
				}
			}

			Text {
				color: "white"
				text: "camera"
				anchors.top: parent.top
				anchors.topMargin: 3
				anchors.left: parent.right
				transformOrigin: Item.TopLeft
				rotation: 90
			}
		}
	}

	Component {
		id: shipProgressBar

		Item {
			implicitWidth: pbar.implicitWidth
			implicitHeight: pbar.implicitHeight

			property real progress: 0

			ProgressBar {
				id: pbar
				maximumValue: Math.PI
				minimumValue: 0
				value: progress
				orientation: Qt.Vertical
				anchors.fill: parent

				style: ProgressBarStyle {
					background: Loader { sourceComponent: pbStyleBackground }

					progress: Rectangle {
						color: "#aaa"
						border.color: "gray"

						Rectangle {
							color: "transparent"
							border.color: "#44ffffff"
							anchors.fill: parent
							anchors.margins: 1
						}
					}
				}
			}

			Text {
				color: "white"
				text: "ship"
				anchors.top: parent.top
				anchors.topMargin: 3
				anchors.left: parent.right
				transformOrigin: Item.TopLeft
				rotation: 90
			}
		}
	}
}
