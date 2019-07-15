import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

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

        from: -1
        to: 99999
        editable: true
        value: 800

        onValueChanged: {

            resizeOpEditor.operation.pix_x = widthSpinBox.value;
        }

        ToolTip.visible: hovered
        ToolTip.text: qsTr("New image width in pixels (-1 : deduce from aspect ratio)")
    }


    Text {
        text: qsTr("New image height :")
    }

    SpinBox {
        id: heightSpinBox

        from: -1
        to: 99999
        editable: true
        value: 800

        onValueChanged: {

            resizeOpEditor.operation.pix_y = heightSpinBox.value;
        }

        ToolTip.visible: hovered
        ToolTip.text: qsTr("New image height in pixels (-1 : deduce from aspect ratio)")
    }

    Text {
        text: qsTr("Fit method :")
    }

    ComboBox {
        id: fitMethodComboBox

        onCurrentTextChanged: {
            operation.fitMode = fitMethodComboBox.currentText
        }

        ToolTip.visible: hovered
        ToolTip.text: qsTr("Adjust the new size to keep aspect ratio, or not.")
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

    onOperationChanged: {

        widthSpinBox.value = operation.pix_x
        heightSpinBox.value = operation.pix_y

        var oldFitMode = operation.fitMode;
        var oldInterpolationMode = operation.interpolationMode

        fitMethodComboBox.model = operation.possibleFitModes
        interpolationMethodComboBox.model = operation.possibleInterpolationModes

        var fitModeIndex = fitMethodComboBox.find(oldFitMode)
        if (fitModeIndex >= 0) {
            fitMethodComboBox.currentIndex = fitModeIndex
        }

        var interpolationModeIndex = interpolationMethodComboBox.find(oldInterpolationMode)
        if (interpolationModeIndex >= 0) {
            interpolationMethodComboBox.currentIndex = interpolationModeIndex
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
        onFitModeChanged: {
            var fitModeIndex = fitMethodComboBox.find(operation.fitMode)
            if (fitModeIndex >= 0) {
                fitMethodComboBox.currentIndex = fitModeIndex
            }
        }
        onInterpolationModeChanged: {
            var interpolationModeIndex = interpolationMethodComboBox.find(operation.interpolationMode)
            if (interpolationModeIndex >= 0) {
                interpolationMethodComboBox.currentIndex = interpolationModeIndex
            }
        }
    }
}
