import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.0

Item {

    property alias buttonText: openButton.text
    property alias filePath: textField.text

    property alias dialogTitle: dialog.title
    property alias selectExisting: dialog.selectExisting
    property alias selectFolder: dialog.selectFolder
    property alias nameFilters: dialog.nameFilters

    height: 40
    width: 120

    TextField {

        id: textField

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: openButton.left

    }

    Button {

        id: openButton

        text: qsTr("Open")

        width: 70
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        onClicked: {
            dialog.visible = true;

        }
    }

    FileDialog {
        id: dialog

        title: qsTr("Please select a file")
        folder: shortcuts.home

        selectMultiple: false

        onAccepted: {
            if (dialog.selectFolder) {
                textField.text = dialog.folder
            } else {
                textField.text = dialog.fileUrl
            }
        }
        onRejected: {
        }
    }

}
