import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

TextFieldStyle {
    placeholderTextColor: "#807f8c8d"
    textColor: "#2980b9"
    font.pixelSize: 20*root.pt
    background: Rectangle {
        radius: 4
        implicitWidth: 360*root.pt
        implicitHeight: 46*root.pt
        border.color: "#e74c3c"
        border.width: 2
        color: "#ecf0f1"
    }
}
