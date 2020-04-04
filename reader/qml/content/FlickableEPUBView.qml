import QtQuick 2.14
import EPUBReader 1.0
import QtWebEngine 1.10

Flickable {
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

    clip: true
    contentWidth: parent.width // Do not scroll in this direction
    contentHeight: Math.max(parent.height, epubWebEngine.contentsSize.height)
    //height: epubWebEngine.contentsSize.height
    pressDelay: 400 // Avoid selection
    boundsBehavior: Flickable.StopAtBounds
    boundsMovement: Flickable.StopAtBounds

    /* TODO add key press handler */

    // FIXME Item is needed for Maemo
    Item {
        id: epubItem

        anchors.fill: parent

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

            function calculateWidth(contentWidth) {
                /*
                if (contentWidth <= 0) {
                    //Get the content to expand
                    return 10
                }
                //Use the available space
                if (flickable.width <= contentWidth) {
                    return flickable.width
                }

                return contentWidth
                */
                return flickable.width
            }


            function calculateHeight(contentHeight) {
                if (contentHeight <= 0) {
                    //Get the content to expand
                    return flickable.height
                }
                //Use the available space
                if (flickable.height >= contentHeight) {
                    return flickable.height
                }

                //return contentHeight
                return 9000
            }

            width: calculateWidth(contentsSize.width)
            height: calculateHeight(contentsSize.height)

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
            console.log("Height Item ", height)
        }

        onWidthChanged: {
            console.log("Width Item ", width)
        }
    }

    onHeightChanged: {
        console.log("Height Flickable ", height, "x", width)
    }

    onWidthChanged: {
        console.log("Width Flickable ", height, "x", width)
    }

    onContentHeightChanged: {
        console.log("Content height Flickable", contentHeight)
        resizeContent(contentWidth, contentHeight, Qt.point(0, 0))
    }

    onContentWidthChanged: {
        console.log("Content witdth Flickable", contentWidth)
        //resizeContent()
    }

    /*
    onContentXChanged: {
        console.log("contentX ", contentX)
    }

    onContentYChanged: {
        console.log("contentY ", contentY)
    }
    */
}
