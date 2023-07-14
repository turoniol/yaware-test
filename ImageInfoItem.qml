import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

Column {
    property string percent
    property string imageData
    property string date
    property string time

    property int rectWidth
    property int rectHeight

    function getImgSource() {
        return imageData.length > 0 ? "data:image/png;base64," + imageData : ""
    }

    Rectangle {
        width: rectWidth
        height: rectHeight
        border.color: "red"

        Dialog {
            id: screenshotDialog
            visible: false

            contentItem: Rectangle {
                implicitWidth: 1200
                implicitHeight: 800

                Image {
                    anchors.fill: parent
                    source: getImgSource()
                }
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true

            onPressed: function () {
                screenshotDialog.visible = true;
            }
        }

        Image {
            anchors.fill: parent
            source: getImgSource()
        }

        Rectangle {
            anchors.fill: parent
            color: "#222222"
            opacity: 0.5
            visible: mouseArea.containsMouse


            Text {
                text: "0%"
                color: "yellow"
                font.pointSize: 30

                anchors.centerIn: parent

            }

            Column {

                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                Repeater {
                    model: [time, date]
                    delegate: Text {
                        text: modelData
                        color: "white"
                        font.pointSize: 20
                    }
                }
            }

        }
    }
}
