import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0


SwipeView {

    property alias imageListIndex: imageListView.currentIndex
    property alias imageCount: imageListView.count
    property alias errorCount: errorListView.count

    id: root

    orientation: Qt.Vertical
    interactive: false
    currentIndex: 0

    signal imageProcesingTriggered()
    signal errorDisplayClosed()

    function resetErrorDisplay() {
        if (root.currentIndex != 0) {
            root.currentIndex = 0;
            errorDisplayClosed();
        }

    }

    function setErrorDisplay() {
        if (errorListView.count > 0) {
            root.currentIndex = 1;
        } else {
            resetErrorDisplay();
        }

    }

    Page {

        ColumnLayout {
            spacing: 0
            anchors.fill: parent

            ListView {

                id: imageListView

                Layout.fillWidth: true
                Layout.fillHeight: true

                model: piwapp.images

                spacing: 0

                Component {
                    id: imageEntryDelegate

                    Rectangle {

                        id: rectContent

                        height: 70
                        color: (index === imageListView.currentIndex) ? "#f4a649" : "#cccccc"
                        anchors.left: parent.left
                        anchors.right: parent.right

                        MouseArea {
                            id: noteMouseArea
                            anchors.fill: parent
                            z: 2
                            propagateComposedEvents: true

                            onClicked: mouse.accepted = false;
                            onPressed: {
                                imageListView.currentIndex = index
                                mouse.accepted = false;
                            }
                            onReleased: mouse.accepted = false;
                            onDoubleClicked: mouse.accepted = false;
                            onPositionChanged: mouse.accepted = false;
                            onPressAndHold: mouse.accepted = false;
                        }

                        Image {
                            id: imIcon

                            fillMode: Image.PreserveAspectFit

                            x: 0
                            y: 0
                            height: rectContent.height
                            width: rectContent.height

                            source: "qrc:/icons/image.svg"
                        }

                        Text {
                            id: title

                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            anchors.left: imIcon.right

                            text: display
                            color: (index === imageListView.currentIndex) ? "white" : "#777777"
                            Layout.fillWidth: true
                        }

                    }
                }

                delegate: imageEntryDelegate

            }

            Item {
                Layout.fillWidth: true
                visible: false

                height: 70

                Button {

                    id: startProcessButton
                    text: qsTr("Process images")

                    anchors.centerIn: parent

                    enabled: (imageListView.count > 0)

                    onClicked: {
                        root.imageProcesingTriggered();
                    }

                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Process the images in the queue")

                }
            }

        }
    }

    Page {

        ColumnLayout {
            spacing: 0
            anchors.fill: parent

            ListView {

                id: errorListView

                Layout.fillWidth: true
                Layout.fillHeight: true

                model: piwapp.errors

                spacing: 0

                Component {
                    id: errorDisplayDelegate

                    Item {
                        id: errorBox
                        property int margin: 8

                        height: msgRect.childrenRect.height + 2*margin
                        width: parent.width

                        Rectangle {

                            anchors.fill: errorBox

                            color: "#e59b9b"
                            visible: mouseDetec.containsMouse

                        }

                        Image {
                            id: imIcon

                            fillMode: Image.PreserveAspectFit

                            x: errorBox.margin
                            y: errorBox.margin
                            height: msgRect.childrenRect.height
                            width: 30

                            source: "qrc:/icons/warning.svg"
                        }

                        ColumnLayout {
                            id: msgRect

                            y: errorBox.margin
                            anchors.left: imIcon.right
                            anchors.right: parent.right
                            anchors.leftMargin: errorBox.margin
                            anchors.rightMargin: errorBox.margin

                            Text {
                                id: errImg
                                text: image
                                color: '#bb0202'

                                Layout.fillWidth: true
                            }

                            Text {
                                id: errOp
                                text: operationInfos
                                visible: hasOperation

                                Layout.fillWidth: true
                            }

                            Text {
                                id: errInfos
                                text: infos

                                Layout.fillWidth: true
                            }
                        }

                        MouseArea {
                            id: mouseDetec

                            anchors.fill: errorBox
                            hoverEnabled: true

                            onClicked: {
                                piwapp.errors.removeErrorAt(index);
                            }
                        }
                    }
                }

                delegate: errorDisplayDelegate

                onCountChanged: {
                    if (errorListView.count == 0) {
                        root.resetErrorDisplay();
                    }
                }

            }


            Item {
                Layout.fillWidth: true

                height: 70

                Button {

                    id: leaveErrorAreaButton
                    text: qsTr("Ok")

                    anchors.centerIn: parent

                    enabled: (errorListView.count > 0)

                    onClicked: {
                        root.resetErrorDisplay();
                    }

                }
            }
        }
    }

}
