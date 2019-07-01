import QtQuick 2.0

DropArea {
    id: root
    property int dropIndex
    property int dragAreasShift : 30

    Rectangle {
        id: dropIndicator

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: dragAreasShift

        height: 4
        opacity: root.containsDrag ? 0.8 : 0.0
        color: "#222222"
    }
}
