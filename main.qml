import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

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

        model: ImageModel { }
        delegate: ImageInfoItem {
            percent: model.name
            rectWidth: elementSize; rectHeight: elementSize
        }
    }

    footer: Button {
        property bool isActive: false

        text: "Start"

        onPressed: {
            isActive = !isActive
            text = isActive ? "Stop" : "Start"
        }
    }
}
