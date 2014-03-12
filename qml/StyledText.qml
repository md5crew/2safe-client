import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Text {
    textFormat: Text.StyledText
    onLinkActivated: {
        Qt.openUrlExternally(link)
    }
    font.pixelSize: 16*root.pt
    color: "white"
    linkColor: "#e74c3c"
    text: "Text goes <a href='http://ya.ru'>here</a>"
}
