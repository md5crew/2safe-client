import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.0

Item {
    id: root
    width: 540
    height: 380

    property double pt: Math.min(root.height / 380.0, root.width / 540.0)

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

    FileDialog {
        id: dirDialog
        visible: false
        folder: homepath
        selectExisting: true
        selectFolder: true
        selectMultiple: false
        title: "Please choose a directory to sync"
        onAccepted: {
            syncpath.text = dirDialog.folder
            syncpath.text = syncpath.text.replace("file://", "")
        }
        onRejected: {
            syncpath.text = homepath + "/2safe"
        }
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

            Text {
                color: "#e74c3c"
                Layout.alignment: Qt.AlignHCenter
                font.pixelSize: 18*root.pt
                text: "Choose a directory to be synced with the 2Safe"
            }

            RowLayout {
                Layout.preferredWidth: 360*root.pt
                TextField {
                    id: syncpath
                    Layout.fillWidth: true
                    text: homepath + "/2safe"
                    style: TextFieldStyle {
                        textColor: "#34495e"
                        font.pixelSize: 12*root.pt
                        background: Rectangle {
                            radius: 4
                            implicitHeight: 24*root.pt
                            border.color: "#2980b9"
                            border.width: 2
                            color: "#ecf0f1"
                        }
                    }
                }

                Button {
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: dirDialog.visible = true
                    style: ButtonStyle {
                        label: Text {
                            text: "Browse"
                            color: control.pressed ? Qt.darker("white") : "white"
                            font.pixelSize: 12*root.pt
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            implicitWidth: 70*root.pt
                            implicitHeight: 24*root.pt
                            radius: 2
                            border.width: control.pressed ? 1 : 0
                            border.color: "#2c3e50"
                            gradient: Gradient {
                                GradientStop { position: 0 ; color: control.pressed ? Qt.darker("#3498db") : "#3498db" }
                                GradientStop { position: 1 ; color: control.pressed ? Qt.darker("#2980b9") : "#2980b9" }
                            }
                        }
                    }
                }
            }

            Button {
                Layout.alignment: Qt.AlignHCenter
                //onClicked: XXX
                style: ButtonStyle {
                    label: Text {
                        text: "Let's dock!"
                        color: control.pressed ? Qt.darker("white") : "white"
                        font.pixelSize: 18*root.pt
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        implicitWidth: 120*root.pt
                        implicitHeight: 40*root.pt
                        radius: 2
                        border.width: control.pressed ? 1 : 0
                        border.color: "#333"
                        gradient: Gradient {
                            GradientStop { position: 0 ; color: control.pressed ? Qt.darker("#e67e22") : "#e67e22" }
                            GradientStop { position: 1 ; color: control.pressed ? Qt.darker("#d35400") : "#d35400" }
                        }
                    }
                }
            }
        }
    }
}
