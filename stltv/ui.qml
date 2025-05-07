import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 1280
    height: 720
    title: "Custom TV OS UI"

    Rectangle {
        anchors.fill: parent
        color: "#202020"

        // Menu Button
        Button {
            id: playButton
            text: "Play"
            anchors.centerIn: parent
            font.pixelSize: 40
            onClicked: {
                // Handle play action
            }
        }

        // Example of a simple animation on focus change
        states: State {
            name: "highlighted"
            when: playButton.activeFocus
            PropertyChanges {
                target: playButton
                scale: 1.2
            }
        }
        transitions: Transition {
            NumberAnimation { property: "scale"; duration: 200 }
        }
    }
}
