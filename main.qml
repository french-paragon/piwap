import QtQuick 2.7
import QtQuick.Controls 2.10
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

import "./qmlcomponents/styles" as PiwapStyles
import "./qmlcomponents/controls"

ApplicationWindow {
    id: app
    visible: true
    width: 560
    height: 640
    title: qsTr("Piwap - Pictures Warping App") + piwapp.is_saved ? '' : '*'

    function newActionList() {
        piwapp.newOperationsList();
    }

    function save() {
        if (piwapp.opened_file.length !== 0) {
            piwapp.saveOperations(piwapp.opened_file);
        } else {
            saveActionsFileDialog.open();
        }
    }

    function saveAs() {
        saveActionsFileDialog.open();
    }

    function open() {
        loadActionsFileDialog.open();
    }

    function openFile(inFile) {
        piwapp.loadOperations(inFile)
    }

    header: ToolBar {

        property int icons_size: 30
        id: toolBar

        background: Rectangle {
            id: backgroundRect
            color: "#999999"
            implicitWidth: 40
            implicitHeight: toolBar.icons_size + 15
        }

        Rectangle {
            color: "#f4a649"
            width: parent.width
            height: 3
            anchors.bottom: parent.bottom
            z: 99
        }

        RowLayout {
            anchors.fill: parent

            HoverableToolButton {

                property bool menuResetFlag: false

                id: fileButton
                icon.width: toolBar.icons_size
                icon.height: toolBar.icons_size
                icon.source: "qrc:/icons/logo.svg"
                icon.color: "transparent"
                text: qsTr("File")
                autoOpenable: true

                onClicked: {
                    if (fileButton.isOpen) {
                        fileMenu.popup(fileButton, 0, fileButton.height);
                    }

                }

                Menu {
                    id: fileMenu
                    title: qsTr("File")

                    Component {
                        id: recentFileActionTemplate

                        Action {
                            property string fileUrl: ''
                            enabled: fileUrl.length > 0

                            onTriggered: {
                                if (fileUrl.length > 0) {
                                    app.openFile(fileUrl);
                                }
                            }
                        }
                    }

                    function populateRecentFiles() {

                        while(openRecentMenu.contentData.length > 0) {
                            openRecentMenu.removeItem(openRecentMenu.contentData[0]);
                        }

                        var recentFileList = piwapp.recent_files;
                        var act;

                        var anyRecentFile = false;
                        for (var s in recentFileList) {
                            var fileUrl = recentFileList[s];
                            var fileName = fileUrl.substring(0, fileUrl.length - piwapp.project_file_ext.length);

                            var id = fileName.lastIndexOf('/');

                            if (id >= 0) {
                                fileName = fileName.substring(id+1);
                            }

                            act = recentFileActionTemplate.createObject(openRecentMenu, {fileUrl: fileUrl, text: fileName});

                            if (act !== null) {
                                openRecentMenu.addAction(act);
                                anyRecentFile = true;
                            }
                        }

                        if (!anyRecentFile) {
                            act = recentFileActionTemplate.createObject(openRecentMenu, {fileUrl: '', text: qsTr("No recent files...")});
                            openRecentMenu.addAction(act);
                        }

                    }

                    Action {
                        text: qsTr("&New...")

                        onTriggered: {
                            app.newActionList();
                        }
                    }
                    Action {
                        text: qsTr("&Open...")

                        onTriggered: {
                            app.open();
                        }
                    }
                    Action {
                        text: qsTr("&Save")
                        enabled: !piwapp.is_saved && actionManagement.actionsCount > 0

                        onTriggered: {
                            app.save();
                        }
                    }
                    Action {
                        text: qsTr("Save &As...")
                        enabled: actionManagement.actionsCount > 0

                        onTriggered: {
                            app.saveAs();
                        }
                    }
                    MenuSeparator { }

                    Menu {
                        id: openRecentMenu
                        title: qsTr("&Open recent...")
                    }

                    MenuSeparator { }
                    Action {
                        text: qsTr("&Quit")

                        onTriggered: {
                            Qt.quit();
                        }
                    }

                    onClosed: {
                        if(!fileButton.containMouse) {
                            fileButton.isOpen = false;
                        }
                    }

                    onAboutToShow: {
                        fileMenu.populateRecentFiles();
                    }
                 }
            }

            HoverableToolButton {

                id: saveButton
                icon.width: toolBar.icons_size
                icon.height: toolBar.icons_size
                icon.source: "qrc:/icons/save.svg"
                icon.color: "transparent"
                enabled: !piwapp.is_saved && actionManagement.actionsCount > 0
                opacity: (saveButton.enabled) ? 1.0 : 0.5

                onClicked: {
                    app.save();
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("Save operation list")
            }

            HoverableToolButton {

                id: saveAsButton
                icon.width: toolBar.icons_size
                icon.height: toolBar.icons_size
                icon.source: "qrc:/icons/filesaveas.svg"
                icon.color: "transparent"
                enabled: actionManagement.actionsCount > 0
                opacity: (saveAsButton.enabled) ? 1.0 : 0.5

                onClicked: {
                    app.saveAs();
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("Save operation list as")
            }



            HoverableToolButton {

                id: openButton
                icon.width: toolBar.icons_size
                icon.height: toolBar.icons_size
                icon.source: "qrc:/icons/fileopen.svg"
                icon.color: "transparent"

                onClicked: {
                    app.open();
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("Load operation list")
            }

            HoverableToolButton {
                id: addActionButton
                icon.width: toolBar.icons_size
                icon.height: toolBar.icons_size
                icon.source: "qrc:/icons/add_operation.svg"
                icon.color: "transparent"
                autoOpenable: true

                onClicked: {
                    if (addActionButton.isOpen) {
                        swipeView.setCurrentIndex(0);
                        actionManagement.currentIndex = 0;
                    } else {
                        actionManagement.currentIndex = 1;
                    }
                }

                Connections {
                    target: swipeView
                    onCurrentIndexChanged : {
                        if (swipeView.currentIndex == 1) {
                            addActionButton.isOpen = false;
                            actionManagement.currentIndex = 1;
                        } else {
                            addActionButton.autoOpenable = true;
                        }
                    }
                }

                Connections {
                    target: actionManagement
                    onCurrentIndexChanged : {
                        if (actionManagement.currentIndex == 1) {
                            addActionButton.isOpen = false;
                        }
                    }
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("Add an operation to the queue")
            }

            HoverableToolButton {
                id: removeActionButton
                icon.width: toolBar.icons_size
                icon.height: toolBar.icons_size
                icon.source: "qrc:/icons/remove_operation.svg"
                icon.color: "transparent"
                enabled: swipeView.currentIndex == 0 && actionManagement.currentIndex == 1
                opacity: (removeActionButton.enabled) ? 1.0 : 0.5

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

            HoverableToolButton {
                id: addImageButton
                icon.width: toolBar.icons_size
                icon.height: toolBar.icons_size
                icon.source: "qrc:/icons/add_image.svg"
                icon.color: "transparent"

                onClicked: {
                    swipeView.setCurrentIndex(1);
                    loadImageFileDialog.open();
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("Add an image to the queue")
            }

            HoverableToolButton {
                id: removeImageButton
                icon.width: toolBar.icons_size
                icon.height: toolBar.icons_size
                icon.source: "qrc:/icons/remove_image.svg"
                icon.color: "transparent"
                enabled: swipeView.currentIndex == 1
                opacity: (removeImageButton.enabled) ? 1.0 : 0.5

                onClicked: {
                    if (swipeView.currentIndex === 1) {
                        piwapp.images.removeImage(imageManagement.imageListIndex)
                    }
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("Remove selected image from the queue")
            }

            HoverableToolButton {
                id: runActionsButton
                icon.width: toolBar.icons_size
                icon.height: toolBar.icons_size
                icon.source: "qrc:/icons/run_operations.svg"
                icon.color: "transparent"
                enabled: imageManagement.imageCount > 0
                opacity: (runActionsButton.enabled) ? 1.0 : 0.5

                onClicked: {
                    piwapp.treatImages()
                    if (imageManagement.errorCount > 0) {
                        swipeView.currentIndex = 1
                        imageManagement.setErrorDisplay();
                    }
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("Treat images in the queue")
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

            onErrorDisplayClosed: {
                actionManagement.currentIndex = 1;
                swipeView.currentIndex = 0;
            }
        }

        onCurrentIndexChanged: {
            if (currentIndex == 0) {
                imageManagement.resetErrorDisplay();
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
            app.openFile(loadActionsFileDialog.fileUrl)
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
            text: qsTr("Images") + ((imageManagement.imageCount > 0) ? "(" + imageManagement.imageCount + ")" : "")
            background: PiwapStyles.PiwapTabButtonStyle {

            }
            contentItem: PiwapStyles.PiwapTabButtonTextStyle {

            }

            ToolTip.visible: hovered
            ToolTip.text: qsTr("Manage the images queue")
        }


    }
}
