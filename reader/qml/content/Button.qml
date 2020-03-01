import QtQuick 2.0

Item {
    id: button
    property alias image: icon.source
    property variant action

    signal clicked

    width: 120
    height: parent.height

    Rectangle {
        visible: mouseArea.pressed
        color: activePalette.highlight

        anchors.fill: parent
    }

    Image {
        id: icon
        anchors.centerIn: parent
        opacity: if (action != undefined) {action.enabled ? 1.0 : 0.4} else 1
        width: 48
        height: 48
        smooth: true

        sourceSize {
            width: 48
            height: 48
        }
    }

    MouseArea {
        id: mouseArea

        anchors {fill: parent}
        onClicked: {
            if (action != undefined && action.enabled)
                action.trigger()
            button.clicked()
        }
    }
}
