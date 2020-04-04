import QtQuick 2.14
import EPUBReader 1.0
import QtWebEngine 1.10

Rectangle {
    id: flickable

    property alias fileName: epubView.fileName
    property alias prevPage: epubView.prevPage
    property alias nextPage: epubView.nextPage
    property alias textSizeMultiplier: epubView.textSizeMultiplier
    property alias defaultFont: epubView.defaultFont
    property alias backgroundIndex: epubView.backgroundIndex
    property alias tocDocument: epubView.tocDocument
    property alias url: epubView.url

    function openTocDocumentRequest(path) {
        epubView.openTocDocumentRequest(path)
    }

    /* TODO add key press handler */

    EPUBView {
        id: epubView

        preferredWidth: flickable.width
        preferredHeight: flickable.height

        onUrlChanged: {
            flickable.contentX = 0
            flickable.contentY = 0
        }
    }

    WebEngineView {
        id: epubWebEngine

        url: epubView.url
        settings.javascriptEnabled: false
        settings.localContentCanAccessFileUrls: false

        anchors.fill: parent

        onContentsSizeChanged: {
            console.log("Contents Size epubWebEngine", contentsSize.height, contentsSize.width)
        }

        onUrlChanged: {
            console.log(url)
        }

        anchors.onTopChanged: {
            console.log(anchors.top, anchors.left, anchors.right, anchors.bottom)
        }

        anchors.onBottomChanged: {
            console.log(anchors.top, anchors.left, anchors.right, anchors.bottom)
        }

        anchors.onLeftChanged: {
            console.log(anchors.top, anchors.left, anchors.right, anchors.bottom)
        }

        anchors.onRightChanged: {
            console.log(anchors.top, anchors.left, anchors.right, anchors.bottom)
        }

        onHeightChanged: {
            console.log("Height epubWebEngine ", height)
        }
    }

    onHeightChanged: {
        console.log("Height Flickable ", height, "x", width)
    }

    onWidthChanged: {
        console.log("Width Flickable ", height, "x", width)
    }
}
