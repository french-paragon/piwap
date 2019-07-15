import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import "qmlcomponents/controls"

SwipeView {

    property alias actionListIndex: actionListView.currentIndex

    id: actionManagement
    orientation: Qt.Vertical
    interactive: false
    currentIndex: 1

    Page {

        ColumnLayout {

            anchors.fill: parent
            spacing: 5

            ListView {
                property int rectMargin : 20

                id: availableActionsListView
                Layout.fillHeight: true
                Layout.fillWidth: true
                spacing: 5
                Layout.margins: 15

                model: piwapp.operationFactoryManager

                Component {
                    id: availableActionsListDelegate

                    Rectangle {

                        height: 50
                        color: (index === availableActionsListView.currentIndex) ? "#f4a649" : "#cccccc"
                        radius: 5
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: availableActionsListView.rectMargin
                        anchors.rightMargin: availableActionsListView.rectMargin

                        MouseArea {
                            id: noteMouseArea
                            anchors.fill: parent
                            z: 2
                            propagateComposedEvents: true

                            hoverEnabled: true

                            onClicked: mouse.accepted = false;
                            onPressed: {
                                availableActionsListView.currentIndex = index
                                mouse.accepted = false;
                            }
                            onReleased: mouse.accepted = false;
                            onDoubleClicked: mouse.accepted = false;
                            onPositionChanged: mouse.accepted = false;
                            onPressAndHold: mouse.accepted = false;

                            ToolTip {
                                id: actionToolTip
                                text: toolTip

                                visible: parent.containsMouse && text != ""
                            }
                        }

                        Text {
                            id: title
                            text: display
                            anchors.centerIn: parent
                            color: (index === availableActionsListView.currentIndex) ? "white" : "black"
                        }

                        Image {
                            id: icon
                            source: iconPath

                            width: 35
                            height: 35

                            smooth: true

                            anchors.right: title.left
                            anchors.rightMargin: 10
                            anchors.verticalCenter: title.verticalCenter
                        }

                    }
                }

                delegate: availableActionsListDelegate
            }

            Item {
                id: buttonsArea
                Layout.fillWidth: true

                height: 70
                Layout.margins: 15

                RowLayout {

                    spacing: 5

                    anchors.centerIn: parent

                    Button {
                        id: validateButton
                        text: qsTr("Add")

                        onClicked: {

                            var selectedTypeId = piwapp.operationFactoryManager.typeIdByRow(availableActionsListView.currentIndex);
                            piwapp.addOpToProject(selectedTypeId);

                            actionManagement.currentIndex = 1;
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Add selected action to the operations queue")
                    }

                    Button {
                        id: cancelButton
                        text: qsTr("Back to actions list")

                        onClicked: {
                            actionManagement.currentIndex = 1;
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Go back to the operations queue")
                    }

                }

            }
        }

    }

    Page {

        id: actionListPage

        ColumnLayout {

            anchors.fill: parent
            spacing: 5

            ListView {
                property int rectMargin : 20

                id: actionListView
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.margins: 15

                model: piwapp.operations

                Component {
                    id: actionListDelegate

                    DraggableListItem {

                        id: dragItem

                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: actionListView.rectMargin
                        anchors.rightMargin: actionListView.rectMargin

                        draggedItemParent: actionListPage
                        dragAreasShift: 25

                        onMoveItemRequested: {
                            piwapp.operations.moveAction(from, to);
                        }

                        onItemClicked: {
                            actionListView.currentIndex = index
                        }

                        Item {

                            id: actionItem

                            anchors.left: parent.left
                            anchors.right: parent.right

                            height: actionHeader.height + actionEditor.height

                            Rectangle {

                                id: actionHeader

                                height: 50
                                color: (index === actionListView.currentIndex) ? "#f4a649" : "#cccccc"
                                anchors.left: parent.left
                                anchors.right: parent.right

                                Text {
                                    id: action_title
                                    text: display
                                    anchors.centerIn: parent
                                    color: (index === actionListView.currentIndex) ? "white" : "black"
                                }

                                Image {
                                    id: icon
                                    source: iconPath

                                    width: 35
                                    height: 35

                                    smooth: true

                                    anchors.left: actionHeader.left
                                    anchors.leftMargin: 10
                                    anchors.verticalCenter: actionHeader.verticalCenter
                                }

                            }

                            Rectangle {

                                property int controlMargin : 10


                                id: actionEditor

                                height: (index === actionListView.currentIndex) ? editorLoader.height + controlMargin*2 : 0
                                visible: index === actionListView.currentIndex
                                y: 50
                                color: "#fef7e9"

                                anchors.left: parent.left
                                anchors.right: parent.right

                                Loader {

                                    id: editorLoader
                                    active: piwapp.operations.opAtRow(index) !== null
                                    source: (editorLoader.active) ? piwapp.operations.opAtRow(index).getPropertiesEditorUrl() : ""

                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.top: parent.top

                                    anchors.leftMargin: actionEditor.controlMargin
                                    anchors.rightMargin: actionEditor.controlMargin
                                    anchors.topMargin: actionEditor.controlMargin

                                    height: (editorLoader.item !== null && typeof(editorLoader.item)!== 'undefined')? editorLoader.item.height: 0

                                    onLoaded: {
                                        editorLoader.item.operation = piwapp.operations.opAtRow(index);
                                    }

                                }

                            }
                        }
                    }
                }

                delegate : actionListDelegate
            }

        }

    }
}
