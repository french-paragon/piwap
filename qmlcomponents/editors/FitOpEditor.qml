import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

import "../controls"


GridLayout {

    property QtObject operation: null
    property int controlsMargin: 5

    id : resizeOpEditor

    anchors.left: parent.left
    anchors.right: parent.right

    columns: 2

    columnSpacing: 5
    rowSpacing: 5

    height: childrenRect.height

    Text {
        text: qsTr("New image width :")
    }

    SpinBox {
        id: widthSpinBox

        from: 1
        to: 99999
        editable: true
        value: resizeOpEditor.operation.pix_x

        onValueChanged: {

            resizeOpEditor.operation.pix_x = widthSpinBox.value;
        }

        ToolTip.visible: hovered
        ToolTip.text: qsTr("New image width in pixels")
    }


    Text {
        text: qsTr("New image height :")
    }

    SpinBox {
        id: heightSpinBox

        from: 1
        to: 99999
        editable: true
        value: resizeOpEditor.operation.pix_y

        onValueChanged: {

            resizeOpEditor.operation.pix_y = heightSpinBox.value;
        }

        ToolTip.visible: hovered
        ToolTip.text: qsTr("New image height in pixels")
    }


    Text {
        text: qsTr("Background color :")
    }

    ColorSelector {
        id: colorSelector

        color: resizeOpEditor.operation.bg
        width: heightSpinBox.width

        onColorChanged: {
            resizeOpEditor.operation.bg = colorSelector.color;
        }
    }

}
