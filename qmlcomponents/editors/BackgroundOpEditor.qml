import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

import "../controls"


GridLayout {

    property QtObject operation: null
    property int controlsMargin: 5

    id : backgroundOpEditor

    anchors.left: parent.left
    anchors.right: parent.right

    columns: 2

    columnSpacing: 5
    rowSpacing: 5

    height: childrenRect.height


    Text {
        text: qsTr("Background color :")
    }

    ColorSelector {
        id: colorSelector

        color: backgroundOpEditor.operation.bg

        onColorChanged: {
            backgroundOpEditor.operation.bg = colorSelector.color;
        }
    }

    onOperationChanged: {

        colorSelector.color = operation.bg
    }

    Connections {
        target: operation

        onBg_changed: {
            colorSelector.color = operation.bg
        }
    }

}
