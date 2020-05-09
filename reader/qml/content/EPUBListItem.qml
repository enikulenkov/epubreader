import QtQuick 2.0
import QtWebEngine 1.10


Flickable {
    id: epubListItem

    property alias url: epubWebEngine.url
    property int backgroundIndex: 0
    property real textSizeMultiplier: 1.0
    property string defaultFont: ""

    contentHeight: epubWebEngine.height
    contentWidth: epubWebEngine.width

    function reload() {
        epubWebEngine.reload()
    }

    WebEngineView {
        id: epubWebEngine

        property int actualHeight: -1
        property int actualWidth: -1

        settings.javascriptEnabled: true
        settings.localContentCanAccessFileUrls: false
        backgroundColor: "grey"

        height: Math.max(actualHeight, epubListItem.height)
        width: Math.max(actualWidth, epubListItem.width)

        onContentsSizeChanged: {
            console.log(index, ": Contents Size epubWebEngine", contentsSize.height, "x", contentsSize.width)
        }

        onUrlChanged: {
            console.log(index, ": url changed", url)
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
            console.log(index, ": Height/Width epubWebEngine ", height, "x", width)
        }

        onWidthChanged: {
            console.log(index, ": Height/Width epubWebEngine ", height, "x", width)
        }

        function setZoomStr() {
            if (epubListItem.textSizeMultiplier !== 1.0) {
                return "document.body.style.zoom = " + epubListItem.textSizeMultiplier + ";";
            } else {
                return "";
            }
        }

        function setBackgrounColorStr() {
            var c;
            switch (epubListItem.backgroundIndex) {
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
                runJavaScript(
                    "document.documentElement.scrollHeight;",
                    function (i_actualPageHeight) {
                        epubWebEngine.actualHeight = i_actualPageHeight;
                    })
                runJavaScript(
                    "document.documentElement.scrollWidth;",
                    function (i_actualPageWidth) {
                        epubWebEngine.actualWidth = i_actualPageWidth;
                    })
            }
        }

        // Open external links in system browser, do not open in WebEngineView
        onNavigationRequested: function(request) {
            if (request.navigationType === WebEngineNavigationRequest.LinkClickedNavigation) {
                Qt.openUrlExternally(request.url);
                request.action = WebEngineNavigationRequest.IgnoreRequest;
            }
        }
    }

}
