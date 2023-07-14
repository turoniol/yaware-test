import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Screenshots 1.0

ApplicationWindow {
    id: mainWindow

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

        model: ScreenshotsModel {
            screenshots: controller.screenshots
        }
        delegate: ImageInfoItem {
            rectWidth: elementSize; rectHeight: elementSize

            percent: model.percentage
            date: model.date
            time: model.time
            imageData: model.hashData
        }
    }

    footer: Button {
        property bool isActive: false

        text: "Start"

        onPressed: {
            isActive = !isActive
            text = isActive ? "Stop" : "Start"

            controller.run()
        }
    }
}
