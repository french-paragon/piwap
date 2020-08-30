import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0


GridLayout {

    property QtObject operation: null
    property int controlsMargin: 5

    id : bevelOpEditor

    anchors.left: parent.left
    anchors.right: parent.right

    columns: 2

    columnSpacing: 5
    rowSpacing: 5

    height: childrenRect.height


    Text {
        text: qsTr("Bevel radius :")
    }

    SpinBox {
        id: radiusSelector
        from: 0
        to: 999
        editable: true
    }

    onOperationChanged: {

        radiusSelector.value = operation.radius
    }

    Connections {
        target: operation

        onRadius_changed: {
            radiusSelector.value = operation.radius
        }
    }

}
