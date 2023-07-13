import QtQuick 2.15
import QtQuick.Controls 2.15

Column {
    property string percent
    property int rectWidth
    property int rectHeight

    Rectangle {
        width: rectWidth
        height: rectHeight
        border.color: "red"

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
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
                    model: ["12:00", "07.13.2023"]
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
