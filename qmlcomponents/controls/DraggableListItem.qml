import QtQuick 2.0

Item {
    id: root

    default property Item contentItem

    // This item will become the parent of the dragged item during the drag operation
    property Item draggedItemParent
    property int dragAreasShift : 30

    signal moveItemRequested(int from, int to)
    signal itemClicked()

    width: contentItem.width
    height: contentItem.height

    // Make contentItem a child of contentItemWrapper
    onContentItemChanged: {
        contentItem.parent = contentItemWrapper;
    }

    Rectangle {
        id: contentItemWrapper
        anchors.fill: parent

        Drag.active: dragArea.drag.active

        Drag.hotSpot {
            x: contentItem.width / 2
            y: root.dragAreasShift
        }

        MouseArea {

            id: dragArea
            anchors.fill: parent

            drag.target: parent
            // Keep the dragged item at the same X position. Nice for lists, but not mandatory
            drag.axis: Drag.YAxis
            // Disable smoothed so that the Item pixel from where we started the drag remains
            // under the mouse cursor
            drag.smoothed: false

            onReleased: {
                if (drag.active) {
                    emitMoveItemRequested();
                }
            }

            onPressed: {
                root.itemClicked();
            }
        }
    }

    Loader {
        id: topDropAreaLoader
        active: model.index === 0

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: contentItem.height - root.dragAreasShift

        height: root.dragAreasShift*2
        sourceComponent: Component {
            DraggableItemDropArea {
                dropIndex: 0
                dragAreasShift: root.dragAreasShift
            }
        }
    }

    DraggableItemDropArea {

        id: dropArea

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: root.dragAreasShift

        dragAreasShift: root.dragAreasShift

        height: contentItem.height
        dropIndex: model.index + 1
    }

    states: [
        State {
            when: dragArea.drag.active
            name: "dragging"

            ParentChange {
                target: contentItemWrapper
                parent: draggedItemParent
            }
            PropertyChanges {
                target: contentItemWrapper
                opacity: 0.5
                anchors.fill: undefined
                width: contentItem.width
                height: contentItem.height
            }
            PropertyChanges {
                target: topDropAreaLoader
                height: 0
                visible: false
                anchors.right: parent.left
            }
            PropertyChanges {
                target: dropArea
                height: 0
                visible: false
                anchors.right: parent.left
            }
        }
    ]

    function emitMoveItemRequested() {

            var dropArea = contentItemWrapper.Drag.target;
            if (!dropArea) {
                return;
            }
            var dropIndex = dropArea.dropIndex;

            // If the dragged item is below the drop target, then decrement dropIndex
            if (model.index < dropIndex) {
                dropIndex--;
            }

            if (model.index === dropIndex) {
                return;
            }

            root.moveItemRequested(model.index, dropIndex);
    }
}
