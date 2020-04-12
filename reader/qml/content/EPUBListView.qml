import QtQuick 2.14
import QtQuick.Controls 2.14
import EPUBReader 1.0
import QtWebEngine 1.10

ListView {
    id: flickable

    property alias fileName: epubView.fileName
    property alias prevPage: epubView.prevPage
    property alias nextPage: epubView.nextPage
    property alias textSizeMultiplier: epubView.textSizeMultiplier
    property alias defaultFont: epubView.defaultFont
    property alias backgroundIndex: epubView.backgroundIndex
    property alias tocDocument: epubView.tocDocument
    property alias url: epubView.url
    property bool __opened: false

    orientation: ListView.Horizontal
    snapMode: ListView.SnapOneItem
    highlightFollowsCurrentItem: true
    highlightRangeMode: ListView.StrictlyEnforceRange
    // Cache 2 pages backwards and 2 pages forward
    cacheBuffer: flickable.width * 2

    function openTocDocumentRequest(path) {
        epubView.openTocDocumentRequest(path)
    }

    /* TODO add key press handler */

    EPUBView {
        id: epubView

        signal settingsChanged

        onUrlChanged: {
            console.log("New URL ", url);
            var idx = getPageIdx(url);
            if (flickable.currentIndex !== idx) {
                if (Math.abs(flickable.currentIndex - idx) == 1) {
                    flickable.currentIndex = idx;
                } else {
                    flickable.positionViewAtIndex(idx, ListView.SnapPosition);
                }
            }
        }

        onDocumentOpened: {
            console.log("Document opened");
            flickable.positionViewAtIndex(getPageIdx(url), ListView.SnapPosition);
            __opened = true;
        }

        onTextSizeMultiplierChanged: {
            settingsChanged()
        }

        onDefaultFontChanged: {
            settingsChanged()
        }

        onBackgroundIndexChanged: {
            settingsChanged()
        }
    }

    Component {
        id: myDelegate

        WebEngineView {
            id: epubWebEngine

            Connections {
                target: epubView
                onSettingsChanged: reload()
            }

            function getUrl() {
                return epubView.getPageUrl(index);
            }

            url: getUrl()
            settings.javascriptEnabled: true
            settings.localContentCanAccessFileUrls: false
            backgroundColor: "grey"

            implicitHeight: flickable.height == 0 ? 1 : flickable.height
            implicitWidth: flickable.width == 0 ? 1 : flickable.width

            onContentsSizeChanged: {
                console.log("Contents Size epubWebEngine", contentsSize.height, contentsSize.width)
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

            // Open external links in system browser, do not open in WebEngineView
            onNavigationRequested: function(request) {
                if (request.navigationType === WebEngineNavigationRequest.LinkClickedNavigation) {
                    Qt.openUrlExternally(request.url);
                    request.action = WebEngineNavigationRequest.IgnoreRequest;
                }
            }
        }
    }

    model: epubView.pagesNum
    delegate: myDelegate


    onHeightChanged: {
        console.log("Height Flickable ", height, "x", width)
    }

    onWidthChanged: {
        console.log("Width Flickable ", height, "x", width)
    }

    onCurrentIndexChanged: {
        console.log("current index", currentIndex);
        if (__opened) {
            console.log(epubView.pagesNum);
            console.log(flickable.flickableDirection);
            epubView.showPage(currentIndex);
        }
    }
}
