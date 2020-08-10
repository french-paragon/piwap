import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

GridLayout {

    property QtObject operation: null
    property int controlsMargin: 5

    anchors.left: parent.left
    anchors.right: parent.right

    columns: 2

    columnSpacing: 5
    rowSpacing: 5

    height: childrenRect.height

    Text {
        text: qsTr("Use checkpoint :")
    }

    ComboBox {
        id: checkpointCombobox

        onCurrentTextChanged: {
            operation.checkpoint = checkpointCombobox.currentText
        }

        textRole: "display"

        ToolTip.visible: hovered
        ToolTip.text: qsTr("Checkpoint to load.")
    }

    onOperationChanged: {

        var oldCheckpoint = operation.checkpoint;

        checkpointCombobox.model = operation.possibleCheckpoints

        var checkpointIndex = checkpointCombobox.find(oldCheckpoint)
        if (checkpointIndex >= 0) {
            checkpointCombobox.currentIndex = checkpointIndex
        }
    }

}
