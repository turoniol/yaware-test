import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Screenshots 1.0

ApplicationWindow {
    width: 1280
    height: 960
    visible: true
    title: qsTr("Hello World!")

    readonly property int cellSize: 300
    readonly property int elementSize: cellSize - 5

    GridView {
        anchors.fill: parent
        anchors.horizontalCenter: parent.horizontalCenter
        cellWidth: cellSize; cellHeight: cellSize

        model: ScreenshotsModel { }
        delegate: ImageInfoItem {
            percent: model.name
            rectWidth: elementSize; rectHeight: elementSize
        }
    }

    footer: RowLayout {
        Rectangle {
            color: "yellow"
            anchors.fill: parent
        }

        RowLayout {
            Text {
                text: "Delay (s)"
            }
            SpinBox {
                from: 0; to: 60
                value: 60
                stepSize: 5
            }
        }

        Button {
            property bool isActive: false

            text: "Start"

            width: 800

            onPressed: {
                isActive = !isActive
                text = isActive ? "Stop" : "Start"
            }
        }
    }
}
