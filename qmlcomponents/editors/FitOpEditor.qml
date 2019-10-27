import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

import "../controls"


GridLayout {

    property QtObject operation: null
    property int controlsMargin: 5

    id : fitOpEditor

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
        value: fitOpEditor.operation.pix_x

        onValueChanged: {

            if (fitOpEditor.operation != null) {
                fitOpEditor.operation.pix_x = widthSpinBox.value;
            }
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
        value: fitOpEditor.operation.pix_y

        onValueChanged: {

            if (fitOpEditor.operation != null) {
                fitOpEditor.operation.pix_y = heightSpinBox.value;
            }
        }

        ToolTip.visible: hovered
        ToolTip.text: qsTr("New image height in pixels")
    }

    Text {
        text: qsTr("Interpolation method :")
    }

    ComboBox {
        id: interpolationMethodComboBox

        onCurrentTextChanged: {
            operation.interpolationMode = interpolationMethodComboBox.currentText
        }

        ToolTip.visible: hovered
        ToolTip.text: qsTr("Interpolation method")
    }

    Text {
        text: qsTr("Allow expand on dimension :")
    }

    ComboBox {
        id: expandModeComboBox

        onCurrentTextChanged: {
            operation.expandMode = expandModeComboBox.currentText
        }

        ToolTip.visible: hovered
        ToolTip.text: qsTr("Allow the image to be extended on one axis.")
    }


    Text {
        text: qsTr("Background color :")
    }

    ColorSelector {
        id: colorSelector

        color: fitOpEditor.operation.bg
        width: heightSpinBox.width

        onColorChanged: {
            fitOpEditor.operation.bg = colorSelector.color;
        }
    }

    onOperationChanged: {

        widthSpinBox.value = operation.pix_x
        heightSpinBox.value = operation.pix_y

        var oldInterpolationMode = operation.interpolationMode
        var oldExpandMode = operation.expandMode
        var oldColor = operation.bg

        interpolationMethodComboBox.model = operation.possibleInterpolationModes
        expandModeComboBox.model = operation.possibleExpandModes

        var interpolationModeIndex = interpolationMethodComboBox.find(oldInterpolationMode)
        if (interpolationModeIndex >= 0) {
            interpolationMethodComboBox.currentIndex = interpolationModeIndex
        }

        var expandModeIndex = expandModeComboBox.find(oldInterpolationMode)
        if (expandModeIndex >= 0) {
            expandModeComboBox.currentIndex = expandModeIndex
        }
    }

    Connections {
        target: operation
        onPix_x_changed: {
            widthSpinBox.value = operation.pix_x
        }
        onPix_y_changed: {
            heightSpinBox.value = operation.pix_y
        }
        onBg_changed: {
            colorSelector.color = operation.bg
        }

        onInterpolationModeChanged: {
            var interpolationModeIndex = interpolationMethodComboBox.find(operation.interpolationMode)
            if (interpolationModeIndex >= 0) {
                interpolationMethodComboBox.currentIndex = interpolationModeIndex
            }
        }
    }

}
