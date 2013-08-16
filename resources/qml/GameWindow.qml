import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Particles 2.0
import QtQuick.Layouts 1.0

import Precursors.Networking 1.0

import Horde3D 1.0

import "../js/logging.js" as Logging
import "../js/game.js" as Game

Horde3DWindow {
    id: gameWindow

    width: 1024; height: 768
    color: "black"

    property var logger: new Logging.Logger("mainWindow")
    property var game: Game

    property var headingSpeed: 1
    property var pitchSpeed: 1

    property var headingVel: 0
    property var pitchVel: 0

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
				// We have to copy the `id` key, due to shadowing in the ListView.
				character['charID'] = character['id'];

                charList.append(character);
			}); // end forEach

			// Finally, we need to set the initial value of the "currently selected" id.
			charListView.currentItemID = characters[0]['id'];
        } // end onCharListReply
    } // end onConnected

    function onFPSChanged() {
        onFPSChanged.fpsWindow = onFPSChanged.fpsWindow || [];
        onFPSChanged.fpsSum = onFPSChanged.fpsSum || 0;

        onFPSChanged.fpsSum += mainWindow.fps;
        onFPSChanged.fpsWindow.push(mainWindow.fps);

        if(onFPSChanged.fpsWindow.length > 100)
        {
            onFPSChanged.fpsSum -= onFPSChanged.fpsWindow.shift();
        }

        var fps = (onFPSChanged.fpsSum / onFPSChanged.fpsWindow.length).toFixed(1);
        while(fps.length < 6)
        {
            fps = ' ' + fps;
        }
        fpsText.text = "FPS: " + fps;
    } // end onFPSChanged
    onFpsChanged: onFPSChanged()

    FocusScope {
        focus: true
        anchors.fill: parent

        Timer {
            id: rotateTimer
            interval: 16
            running: false
            repeat: true
            onTriggered: {
                if(headingVel == 0 && pitchVel == 0)
                {
                    rotateTimer.stop();
                }
                else
                {
                    horde3d.avatar.rotateHeading(headingVel);
                    horde3d.avatar.rotatePitch(pitchVel);
                }
            }
        }

        function doKeyShit(event, release) {
            var factor = 1;
            if(release)
            {
                factor = -1;
            }

            // On OSX, Qt 5.1 has a bug where event.isAutoRepeat isn't correctly set, and it segfaults when you check
            // it too much.
            if(Qt.platform != "osx" && event.isAutoRepeat)
            {
                factor = 0;
            }

            switch(event.key) {
                case Qt.Key_MediaTogglePlayPause:
                    if(Qt.platform == "osx")
                    {
                        pitchVel = 0;
                        headingVel = 0;
                    }
                    break;
                case Qt.Key_F8:
                    if(Qt.platform != "osx")
                    {
                        pitchVel = 0;
                        headingVel = 0;
                    }
                    break;

                case Qt.Key_F10:
					gameWindow.close();
                    break;

				case Qt.Key_F1:
					if(!release)
					{
						gameWindow.grabMouse = !gameWindow.grabMouse;
					}
					break;

                case Qt.Key_Up:
                    pitchVel += factor * pitchSpeed;
                    break;
                case Qt.Key_Down:
                    pitchVel -= factor * pitchSpeed;
                    break;

                case Qt.Key_Left:
                    headingVel -= factor * headingSpeed;
                    break;
                case Qt.Key_Right:
                    headingVel += factor * headingSpeed;
                    break;

                default:
                    return;
            }

            rotateTimer.start();
            event.accepted = true;
        }

        Keys.onPressed: doKeyShit(event, false)
        Keys.onReleased: doKeyShit(event, true)

        MouseArea {
            id: wrapper
            anchors.fill: parent

			Connections {
				target: controls.context('flightsim').digitalSlot('fire');
				onStateChanged: {
					console.log("Got 'fire'.", controls.context('flightsim').digitalSlot('fire').state);
				}
			}

			Connections {
				target: controls.context('flightsim').digitalSlot('coast');
				onStateChanged: {
					console.log("Got 'coast'.", controls.context('flightsim').digitalSlot('coast').state);
				}
			}

			Connections {
				target: controls.context('flightsim').digitalSlot('headlights');
				onStateChanged: {
					console.log("Got 'headlights'.", controls.context('flightsim').digitalSlot('headlights').state);
				}
			}

			Connections {
				target: controls.context('flightsim').analogSlot('heading');
				onValueChanged: {
					//console.log("Got onValueChanged for 'heading'.");
                    mainWindow.camDolly.rotateHeading(controls.context('flightsim').analogSlot('heading').value);
				}
			}

			Connections {
				target: controls.context('flightsim').analogSlot('pitch');
				onValueChanged: {
					//console.log("Got onValueChanged for 'pitch'.");
                    mainWindow.camDolly.rotatePitch(controls.context('flightsim').analogSlot('pitch').value);
				}
			}

			Connections {
				target: controls.context('flightsim').analogSlot('roll');
				onValueChanged: {
					//console.log("Got onValueChanged for 'roll'.");
                    mainWindow.camDolly.rotateRoll(controls.context('flightsim').analogSlot('roll').value);
				}
			}

            function updateRotation(mouse) {
                var mouseSensitivity = -settings.get('mouseSensitivity', 0.1);
                if(updateRotation.lastX !== undefined && updateRotation.lastY !== undefined)
                {
                    mainWindow.camDolly.rotateHeading((mouse.x - (updateRotation.lastX || mouse.x)) * mouseSensitivity);
                    mainWindow.camDolly.rotatePitch((mouse.y - (updateRotation.lastY || mouse.y)) * mouseSensitivity);
                } // end if

                updateRotation.lastX = mouse.x;
                updateRotation.lastY = mouse.y;
            }

            onPressed: {
                updateRotation.lastX = mouse.x;
                updateRotation.lastY = mouse.y;
            }
            onPositionChanged: {
                updateRotation(mouse);
            }
        }

        Text {
            id: fpsText
            anchors.top: parent.top
            anchors.right: parent.right

            color: "white"
            font.family: "monospace"
            font.pointSize: 24
            font.weight: Font.DemiBold
        }
    }
}
