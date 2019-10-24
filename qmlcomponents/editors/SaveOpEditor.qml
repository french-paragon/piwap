import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

import "../controls"

ColumnLayout {
    property QtObject operation: null
    property int controlsMargin: 5
    property int labelTopMargin: 10

    id : saveOpEditor

    anchors.left: parent.left
    anchors.right: parent.right

    spacing: 5

    height: childrenRect.height

    Text {
        id: labelFolder
        text: qsTr("Output folder")
    }

    FilePathField {

        id: folderSelectField

        Layout.fillWidth: true

        dialogTitle: qsTr("Select a folder")
        selectFolder: true

        onFilePathChanged: {
            operation.folderUrl = folderSelectField.filePath
        }

    }

    Text {
        id: labelFileName
        text: qsTr("Filename")

        Layout.topMargin: saveOpEditor.labelTopMargin
    }

    TextField {

        id: fileNameField

        Layout.fillWidth: true

        onTextChanged: {
            operation.fileName = fileNameField.text
        }

        ToolTip.visible: hovered
        ToolTip.text: qsTr("New name of the file")

    }

    Text {
        id: labelFileType
        text: qsTr("File type")

        Layout.topMargin: saveOpEditor.labelTopMargin
    }

    TextField {

        id: fileTypeField

        Layout.fillWidth: true

        onTextChanged: {
            operation.fileType = fileTypeField.text
        }

        ToolTip.visible: hovered
        ToolTip.text: qsTr("File extension (type will be deduced from it)")

    }

    RowLayout {

        Layout.fillWidth: true
        Layout.topMargin: saveOpEditor.labelTopMargin

        Text {
            id: qualityLabel
            text: qsTr("Image quality")

            Layout.fillWidth: true
        }

        SpinBox {

            id: qualitySpinBox

            from: 0
            to: 100
            editable: true
            value: 90

            onValueChanged: {

                saveOpEditor.operation.compressionParameter = qualitySpinBox.value;
            }

            ToolTip.visible: hovered
            ToolTip.text: qsTr("Image quality or compression (from 0 to 100)")
        }

    }

    onOperationChanged: {

        folderSelectField.filePath = operation.folderUrl
        fileNameField.text = operation.fileName
        fileTypeField.text = operation.fileType
        qualitySpinBox.value = saveOpEditor.operation.compressionParameter

    }

    Connections {
        target: operation
        onFolderUrlChanged: {
            folderSelectField.filePath = operation.folderUrl
        }
        onFileNameChanged: {
            fileNameField.text = operation.fileName
        }
        onFileTypeChanged: {
            fileTypeField.text = operation.fileType
        }
        onCompressionParameterChanged: {
            qualitySpinBox.value = saveOpEditor.operation.compressionParameter
        }
    }
}
