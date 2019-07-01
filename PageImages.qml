import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Page {

    property alias imageListIndex: imageListView.currentIndex

    id: root

    signal imageProcesingTriggered()

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

            height: 70

            Button {

                id: startProcessButton
                text: qsTr("Process images")

                anchors.centerIn: parent

                enabled: (imageListView.count > 0)

                onClicked: {
                    root.imageProcesingTriggered();
                }

            }
        }

    }
}
