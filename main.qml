import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

import "./qmlcomponents/styles" as PiwapStyles

ApplicationWindow {
    visible: true
    width: 480
    height: 640
    title: qsTr("Piwap - Pictures Warping App") + piwapp.is_saved ? '' : '*';

    header: ToolBar {

        background: Rectangle {
            id: backgroundRect
            color: "#999999"
        }

        RowLayout {
            anchors.fill: parent

            ToolButton {

                id: saveButton
                icon.width: 25
                icon.height: 25
                icon.source: "qrc:/icons/save.svg"
                icon.color: "transparent"
                enabled: !piwapp.is_saved
                opacity: (saveButton.enabled) ? 1.0 : 0.5

                background: Rectangle {
                    color: backgroundRect.color
                }

                onClicked: {
                    console.log("click!")
                    if (piwapp.opened_file.length !== 0) {
                        console.log("'" + piwapp.opened_file + "'")
                        piwapp.saveOperations(piwapp.opened_file);
                    } else {
                        console.log("clock!")
                        saveActionsFileDialog.open();
                    }
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("Save operation list")
            }

            ToolButton {

                id: saveAsButton
                icon.width: 25
                icon.height: 25
                icon.source: "qrc:/icons/filesaveas.svg"
                icon.color: "transparent"
                enabled: actionManagement.actionsCount > 0
                opacity: (saveAsButton.enabled) ? 1.0 : 0.5

                background: Rectangle {
                    color: backgroundRect.color
                }

                onClicked: {
                    saveActionsFileDialog.open();
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("Save operation list as")
            }



            ToolButton {

                id: openButton
                icon.width: 25
                icon.height: 25
                icon.source: "qrc:/icons/fileopen.svg"
                icon.color: "transparent"

                background: Rectangle {
                    color: backgroundRect.color
                }

                onClicked: {
                    loadActionsFileDialog.open();
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("Load operation list")
            }

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

    FileDialog {
        id: saveActionsFileDialog
        title: qsTr("Please select an image actions file")
        folder: shortcuts.home
        nameFilters: [ "Piwap files (*.pictwarpingops)", "All files (*)" ]
        selectExisting: false

        onAccepted: {
            //console.log("You chose: " + actionsFileDialog.fileUrl)
            piwapp.saveOperations(saveActionsFileDialog.fileUrl)
        }
    }

    FileDialog {
        id: loadActionsFileDialog
        title: qsTr("Please select an image actions file")
        folder: shortcuts.home
        nameFilters: [ "Piwap files (*.pictwarpingops)", "All files (*)" ]
        selectExisting: true

        onAccepted: {
            //console.log("You chose: " + loadActionsFileDialog.fileUrl)
            piwapp.loadOperations(loadActionsFileDialog.fileUrl)
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
