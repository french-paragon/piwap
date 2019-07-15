import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

import "./qmlcomponents/styles" as PiwapStyles

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Piwap - Pictures Warping App")

    header: ToolBar {

        background: Rectangle {
            id: backgroundRect
            color: "#999999"
        }

        RowLayout {
            anchors.fill: parent

            ToolButton {
                id: addActionButton
                icon.width: 25
                icon.height: 25
                icon.source: "qrc:/icons/add_operation.svg"
                icon.color: "transparent"

                background: Rectangle {
                    color: backgroundRect.color
                }

                onClicked: {
                    swipeView.setCurrentIndex(0);
                    actionManagement.currentIndex = 0;
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("Add an operation to the queue")
            }

            ToolButton {
                id: removeActionButton
                icon.width: 25
                icon.height: 25
                icon.source: "qrc:/icons/remove_operation.svg"
                icon.color: "transparent"

                background: Rectangle {
                    color: backgroundRect.color
                }

                onClicked: {
                    if (swipeView.currentIndex == 0) {
                        if (actionManagement.currentIndex == 1) {
                            var index = actionManagement.actionListIndex
                            piwapp.removeOpFromProject(index);
                        }
                    }
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("Remove selected operation from the queue")
            }

            Item {
                id: toolBarSpacer
                Layout.fillWidth: true
            }

            ToolButton {
                id: addImageButton
                icon.width: 25
                icon.height: 25
                icon.source: "qrc:/icons/add_image.svg"
                icon.color: "transparent"

                background: Rectangle {
                    color: backgroundRect.color
                }

                onClicked: {
                    swipeView.setCurrentIndex(1);
                    loadImageFileDialog.open();
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("Add an image to the queue")
            }

            ToolButton {
                id: removeImageButton
                icon.width: 25
                icon.height: 25
                icon.source: "qrc:/icons/remove_image.svg"
                icon.color: "transparent"

                background: Rectangle {
                    color: backgroundRect.color
                }

                onClicked: {
                    if (swipeView.currentIndex === 1) {
                        piwapp.images.removeImage(imageManagement.imageListIndex)
                    }
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("Remove selected image from the queue")
            }

        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        PageActions {
            id: actionManagement
        }

        PageImages {
            id: imageManagement

            onImageProcesingTriggered : {
                piwapp.treatImages()
            }
        }
    }

    DropArea {
        id: fileInputArea

        z: 12

        anchors.fill: parent

        onEntered: {
            if (!drag.hasUrls) {
                drag.accepted = false
            }
        }

        onDropped: {
            for (var i in drop.urls) {
                piwapp.images.insertImage(drop.urls[i]);
            }

            swipeView.setCurrentIndex(1);
        }
    }

    FileDialog {
        id: loadImageFileDialog

        title: qsTr("Please select the images you want to treat")
        folder: shortcuts.pictures

        onAccepted: {

            for (var index in loadImageFileDialog.fileUrls) {
                piwapp.images.insertImage(loadImageFileDialog.fileUrls[index]);
            }

        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Actions")
            background: PiwapStyles.PiwapTabButtonStyle {

            }
            contentItem: PiwapStyles.PiwapTabButtonTextStyle {

            }

            ToolTip.visible: hovered
            ToolTip.text: qsTr("Manage the operations queue")
        }

        TabButton {
            text: qsTr("Images")
            background: PiwapStyles.PiwapTabButtonStyle {

            }
            contentItem: PiwapStyles.PiwapTabButtonTextStyle {

            }

            ToolTip.visible: hovered
            ToolTip.text: qsTr("Manage the images queue")
        }


    }
}