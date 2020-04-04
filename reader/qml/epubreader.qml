import QtQuick 2.14
import QtWebEngine 1.10
import "content"
import "content/Icons.js" as Icons

Rectangle {
    id: mainView

    SystemPalette {id: activePalette; colorGroup: SystemPalette.Active}

    color: activePalette.base // TODO maybe window?

    FlickableEPUBView {
        id: epubView

        fileName: mainWindow.fileName
        textSizeMultiplier: settings.textSizeMultiplier
        defaultFont: settings.fontFamily
        backgroundIndex: settings.colorIndex

        anchors {top: parent.top; left: parent.left; right: parent.right; bottom: toolBar.top}

        //onMovementStarted: scrollBar.state = "visible"
        //onMovementEnded: scrollBar.state = ""
    }

    /*
    ScrollBar {
        id: scrollBar
        area: epubView
    }
    */

    Rectangle {
        id: toolBar
        height: 70
        color: activePalette.window

        anchors {bottom: parent.bottom; left: parent.left; right: parent.right}

        Row {
            anchors.fill: parent

            Button {
                id: prevButton
                image: "image://hildon-icon/" + Icons.previousPageIcon
                action: epubView.prevPage
            }

            Button {
                id: tocButton
                image: "image://hildon-icon/" + Icons.showTocIcon

                onClicked: {
                    mainWindow.showToc(epubView.tocDocument)
                }
            }

            Button {
                id: nextButton
                image: "image://hildon-icon/" + Icons.nextPageIcon
                action: epubView.nextPage
            }
        }
        Button {
            id: fullSizeButton
            image: "image://hildon-icon/" + Icons.showFullSizeIcon
            onClicked: {
                mainWindow.fullSize = true
            }

            anchors.right: parent.right
        }
    }

    Item {
        id: fullSizeToolBar
        visible: false
        height: 70
        width: 120

        Button {
            id: normalSizeButton
            image: "image://hildon-icon/" + Icons.showNormalIcon
            opacity: 0.4

            onClicked: {
                mainWindow.fullSize = false
            }
        }

        anchors {bottom: parent.bottom; horizontalCenter: parent.horizontalCenter}
    }

    states: State {
        name: "fullSize";
        when: mainWindow.fullSize
        PropertyChanges {target: fullSizeToolBar; visible: true}
        PropertyChanges {target: toolBar; visible: false}
        PropertyChanges {target: epubView; anchors.bottom: mainView.bottom}
    }

    Component.onCompleted: mainWindow.openTocDocumentRequest.connect(epubView.openTocDocumentRequest)

    Component.onDestruction: {
        settings.saveLastURL(epubView.fileName, epubView.url)
    }
}
