import QtQuick 2.14
import QtQuick.Controls 2.14
import EPUBReader 1.0
import "."

ListView {
    id: listview

    property alias fileName: epubView.fileName
    property alias prevPage: epubView.prevPage
    property alias nextPage: epubView.nextPage
    property alias textSizeMultiplier: epubView.textSizeMultiplier
    property alias defaultFont: epubView.defaultFont
    property alias backgroundIndex: epubView.backgroundIndex
    property alias tocDocument: epubView.tocDocument
    property bool __opened: false

    orientation: ListView.Horizontal
    snapMode: ListView.SnapOneItem
    highlightFollowsCurrentItem: true
    highlightRangeMode: ListView.StrictlyEnforceRange
    // Cache 2 pages backwards and 2 pages forward
    cacheBuffer: listview.width * 2

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
            if (listview.currentIndex !== idx) {
                if (Math.abs(listview.currentIndex - idx) == 1) {
                    listview.currentIndex = idx;
                } else {
                    listview.positionViewAtIndex(idx, ListView.SnapPosition);
                }
            }
        }

        onDocumentOpened: {
            console.log("Document opened");
            listview.positionViewAtIndex(getPageIdx(url), ListView.SnapPosition);
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

        EPUBListItem {
            id: webview

            Connections {
                target: epubView
                onSettingsChanged: {
                    reload();
                }
            }

            url: epubView.getPageUrl(index)
            height: listview.height == 0 ? 100 : listview.height
            width: listview.width == 0 ? 100 : listview.width
            backgroundIndex: epubView.backgroundIndex
            defaultFont: epubView.defaultFont
            textSizeMultiplier: epubView.textSizeMultiplier
        }
    }

    model: epubView.pagesNum
    delegate: myDelegate

    onHeightChanged: {
        console.log("Height ListView ", height, "x", width)
    }

    onWidthChanged: {
        console.log("Width ListView ", height, "x", width)
    }

    onContentWidthChanged: {
        console.log("ListView Content", contentHeight, "x", contentWidth)
    }

    onContentHeightChanged: {
        console.log("ListView Content", contentHeight, "x", contentWidth)
    }

    onCurrentIndexChanged: {
        console.log("current index", currentIndex);
        if (__opened) {
            console.log(epubView.pagesNum);
            console.log(listview.flickableDirection);
            epubView.showPage(currentIndex);
        }
    }
}
