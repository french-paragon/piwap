import QtQuick 2.0
import QtQuick.Controls 2.0

ToolButton {
    property bool autoOpenable: false
    property bool isOpen: false
    property alias containMouse: mArea.containMouse

    background: Rectangle {
        color: isOpen ? "#f4a649" : (mArea.containMouse && enabled ? "#bbbbbb" : "#999999")
    }

    MouseArea {
        property bool containMouse: false

        id: mArea

        anchors.fill: parent
        hoverEnabled: true

        onClicked: {
            if (parent.autoOpenable) {
                parent.isOpen = !parent.isOpen;
            }

            parent.clicked();
        }

        onEntered: {
            containMouse = true;
        }

        onExited: {
            containMouse = false;
        }

        z: 2
    }
}
