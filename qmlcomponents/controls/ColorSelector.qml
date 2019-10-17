import QtQuick 2.0
import QtQuick.Dialogs 1.3

Item {
    property alias color: c_selector.color

    height: 30
    width: 30

    Rectangle {

        id: c_selector

        anchors.centerIn: parent

        height: Math.min(parent.height, parent.width)
        width: height

        radius: height/2

        border.color: '#999999'
        border.width: 1

        color: '#000000'

        MouseArea {
            anchors.fill: parent

            onClicked: {
                colorDialog.open();
            }
        }

        ColorDialog {
            id: colorDialog
            currentColor: c_selector.color

            onAccepted: {
                c_selector.color = colorDialog.color;
            }
        }
    }
}
