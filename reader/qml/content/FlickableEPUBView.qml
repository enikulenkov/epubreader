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

        onUrlChanged: {
            console.log("New URL ", url);
        }

        onTextSizeMultiplierChanged: {
            epubWebEngine.reload()
        }

        onDefaultFontChanged: {
            epubWebEngine.reload()
        }

        onBackgroundIndexChanged: {
            epubWebEngine.reload()
        }
    }

    WebEngineView {
        id: epubWebEngine

        url: epubView.url
        settings.javascriptEnabled: true
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

        function setZoomStr() {
            if (flickable.textSizeMultiplier != 1.0) {
                return "document.body.style.zoom = " + flickable.textSizeMultiplier + ";";
            } else {
                return "";
            }
        }

        function setBackgrounColorStr() {
            var c;
            switch (flickable.backgroundIndex) {
            default:
                c = "white";
                break;
            case 1:
                c = "#f1dc6b"; // yellow
                break;
            case 2:
                c = "grey";
                break;
            }

            return "document.body.style.backgroundColor = '" + c +"';"
        }

        function setBodyMargins() {
            return "document.body.style.setProperty('margin-top', '10px', 'important');" +
                    "document.body.style.setProperty('margin-bottom', '10px', 'important');" +
                    "document.body.style.setProperty('margin-right', '10px', 'important');" +
                    "document.body.style.setProperty('margin-left', '10px', 'important');"
        }

        function setFontStr() {
            if (defaultFont != '') {
                return "var newStyle = document.createElement('style');" +
                        'newStyle.appendChild(document.createTextNode("' +
                        "@font-face { " +
                        "font-family: userFont; " +
                        "src: local('" + defaultFont + "'); " +
                        "}" +
                        '"));' +
                        "document.head.appendChild(newStyle);" +
                        "document.body.style.fontFamily = 'userFont';"
            } else {
                return "";
            }
        }

        onLoadingChanged: {
            if (loadRequest.status == WebEngineView.LoadSucceededStatus) {
                runJavaScript(
                            setFontStr() +
                            setZoomStr() +
                            setBackgrounColorStr() +
                            setBodyMargins()
                );
            }
        }
    }

    onHeightChanged: {
        console.log("Height Flickable ", height, "x", width)
    }

    onWidthChanged: {
        console.log("Width Flickable ", height, "x", width)
    }
}
