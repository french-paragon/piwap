import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

import "../controls"


GridLayout {

    property QtObject operation: null
    property int controlsMargin: 5

    id : watermarkOpEditor

    anchors.left: parent.left
    anchors.right: parent.right

    columns: 2

    columnSpacing: 5
    rowSpacing: 5

    height: childrenRect.height

    Text {
        text: qsTr("Watermark file :")
    }

    FilePathField {

        id: watermarkSelectField

        Layout.fillWidth: true

        dialogTitle: qsTr("Select watermark file")
        selectFolder: false
        nameFilters: [ "Image files (*.jpg *.jpeg *.png *.gif *.bmp *.tif *.tiff)", "all files (*.*)" ]

        onFilePathChanged: {
            operation.watermarkImageName = watermarkSelectField.filePath
        }

    }


    Text {
        text: qsTr("Anchor point :")
    }

    ComboBox {
        id: anchorPointComboBox

        onCurrentTextChanged: {
            operation.anchorMode = anchorPointComboBox.currentText
        }

        ToolTip.visible: hovered
        ToolTip.text: qsTr("Anchor points")
    }


    Text {
        text: qsTr("Blending mode :")
    }

    ComboBox {
        id: blendModeComboBox

        onCurrentTextChanged: {
            operation.blendingMode = blendModeComboBox.currentText
        }

        ToolTip.visible: hovered
        ToolTip.text: qsTr("Blending mode")
    }

    Text {
        text: qsTr("Resize :")
    }

    CheckBox {
        id: resizeCheckBox
        tristate: false

        onCheckStateChanged: {
            watermarkOpEditor.operation.doResize = resizeCheckBox.checked;
        }
    }

    Text {
        text: qsTr("Opacity :")
    }

    SpinBox {
        id: rateSpinBox

        from: 0
        to: 100
        stepSize: 10
        editable: true

        onValueChanged: {

            if (watermarkOpEditor.operation !== null) {
                watermarkOpEditor.operation.rate = rateSpinBox.value/100.;
            }
        }

        ToolTip.visible: hovered
        ToolTip.text: qsTr("Opacity of the watermark")
    }

    onOperationChanged: {

        watermarkSelectField.filePath = operation.watermarkImageName

        var oldBlendMode = operation.blendMode
        var oldAnchorMode = operation.anchorMode

        blendModeComboBox.model = operation.possibleBlendingModes
        anchorPointComboBox.model = operation.possibleAnchorModes

        var blendModeIndex = blendModeComboBox.find(oldBlendMode)
        if (blendModeIndex >= 0) {
            blendModeComboBox.currentIndex = blendModeIndex
        }

        var anchorModeIndex = anchorPointComboBox.find(oldAnchorMode)
        if (anchorModeIndex >= 0) {
            anchorPointComboBox.currentIndex = anchorModeIndex
        }

        resizeCheckBox.checked = operation.doResize;
        rateSpinBox.value = 100*operation.rate
    }

    Connections {
        target: operation
        onRateChanged: {
            rateSpinBox.value = 100*operation.rate
        }

        onBlendingModeChanged: {
            var blendModeIndex = blendModeComboBox.find(operation.blendingMode)
            if (blendModeIndex >= 0) {
                blendModeComboBox.currentIndex = blendModeIndex
            }
        }

        onAnchorModeChanged: {
            var anchorModeIndex = anchorPointComboBox.find(operation.anchorMode)
            if (anchorModeIndex >= 0) {
                anchorPointComboBox.currentIndex = anchorModeIndex
            }
        }
    }

}
