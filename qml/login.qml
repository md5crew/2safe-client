import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.0

Item {
    id: root
    width: 540
    height: 520

    property double pt: Math.min(root.height / 520.0, root.width / 540.0)

    Image {
        id: background
        anchors.fill: parent
        source: "artwork/stardust.png"
        fillMode: Image.Tile
    }

    Image {
        id: logo
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 40*root.pt
        source: "artwork/logo.png"
    }

    Item {
        z: 2
        id: form
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: logo.bottom
        anchors.topMargin: 46*root.pt

        ColumnLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 26*root.pt

            TextField {
                id: login
                Layout.alignment: Qt.AlignHCenter
                placeholderText: "Login"
                style: LargeInputStyle{}
            }
            TextField {
                id: password
                Layout.alignment: Qt.AlignHCenter
                placeholderText: "Password"
                style: LargeInputStyle{}
                echoMode: TextInput.Password
            }
            CheckBox {
                visible: false
                id: tos
                Layout.alignment: Qt.AlignHCenter
                style: CheckBoxStyle {
                    indicator: Rectangle {
                        implicitWidth: 18*root.pt
                        implicitHeight: 18*root.pt
                        radius: 3
                        border.color: "#e74c3c"
                        border.width: 3
                        color: "#ecf0f1"
                        Rectangle {
                            visible: control.checked
                            color: "#2980b9"
                            anchors.margins: 5*root.pt
                            anchors.fill: parent
                        }
                    }
                    label: Text {
                        textFormat: Text.StyledText
                        onLinkActivated: {
                            Qt.openUrlExternally(link)
                        }
                        font.pixelSize: 14*root.pt
                        color: "white"
                        linkColor: "#e74c3c"
                        text: "I have read and agree to the <a href='http://2safe.com'>Terms of Service</a>"
                    }
                }
            }
            Button {
                Layout.alignment: Qt.AlignHCenter
                onClicked: Qt.quit()
                style: ButtonStyle {
                    label: Text {
                        text: "Sign In"
                        color: control.pressed ? Qt.darker("white") : "white"
                        font.pixelSize: 18*root.pt
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        implicitWidth: 120*root.pt
                        implicitHeight: 40*root.pt
                        border.width: control.pressed ? 1 : 0
                        border.color: "#333"
                        radius: 3
                        gradient: Gradient {
                            GradientStop { position: 0 ; color: control.pressed ? Qt.darker("#e67e22") : "#e67e22" }
                            GradientStop { position: 1 ; color: control.pressed ? Qt.darker("#d35400") : "#d35400" }
                        }
                    }
                }
            }
            Rectangle {
                color: "#ecf0f1"
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 360*root.pt
                Layout.preferredHeight: 1
            }
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 360*root.pt
                StyledText {
                    Layout.alignment: Qt.AlignLeft
                    text: "<a href='https://www.2safe.com/restore/'>forgot my password /</a>"
                }
                StyledText {
                    Layout.alignment: Qt.AlignRight
                    text: "<a href='http://2safe.com'>sign up /</a>"
                }
            }
        }
    }
}
