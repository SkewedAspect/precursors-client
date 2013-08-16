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

	Connections {
		target: controls.context('flightsim').digitalSlot('quit');
		onStateChanged: {
			logger.info("Exiting...");
			gameWindow.close();
		}
	}

	Connections {
		target: controls.context('flightsim').digitalSlot('grabMouse');
		onStateChanged: {
			gameWindow.grabMouse = controls.context('flightsim').digitalSlot('grabMouse').state;
		}
	}

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
			logger.debug("Got onValueChanged for 'heading': %1", controls.context('flightsim').analogSlot('heading').value);
			mainWindow.camDolly.rotateHeading(controls.context('flightsim').analogSlot('heading').value);
		}
	}

	Connections {
		target: controls.context('flightsim').analogSlot('pitch');
		onValueChanged: {
			logger.debug("Got onValueChanged for 'pitch': %1", controls.context('flightsim').analogSlot('pitch').value);
			mainWindow.camDolly.rotatePitch(controls.context('flightsim').analogSlot('pitch').value);
		}
	}

	Connections {
		target: controls.context('flightsim').analogSlot('roll');
		onValueChanged: {
			logger.debug("Got onValueChanged for 'roll': %1", controls.context('flightsim').analogSlot('roll').value);
			mainWindow.camDolly.rotateRoll(controls.context('flightsim').analogSlot('roll').value);
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
