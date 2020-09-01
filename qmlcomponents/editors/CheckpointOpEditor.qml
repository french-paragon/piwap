import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

GridLayout {

    id: checkpointOpEditor

    property QtObject operation: null
    property int controlsMargin: 5

    anchors.left: parent.left
    anchors.right: parent.right

    columns: 2

    columnSpacing: 5
    rowSpacing: 5

    height: childrenRect.height

    Text {
        text: qsTr("Name :")
    }

    TextField {

        id: nameField

        ToolTip.visible: hovered
        ToolTip.text: "Name of the checkpoint"

        onTextChanged: {

            if (checkpointOpEditor.operation != null) {
                checkpointOpEditor.operation.name = nameField.text;
            }
        }

    }

    onOperationChanged: {

        nameField.text = operation.name
    }

}
