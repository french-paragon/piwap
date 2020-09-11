import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

import "../controls"


GridLayout {

	property QtObject operation: null
	property int controlsMargin: 5

	id : cropOpEditor

	anchors.left: parent.left
	anchors.right: parent.right

	columns: 2

	columnSpacing: 5
	rowSpacing: 5

	height: childrenRect.height

	Text {
		text: qsTr("Cropped image width :")
	}

	SpinBox {
		id: widthSpinBox

		from: 1
		to: 99999
		editable: true
		value: cropOpEditor.operation.width

		onValueChanged: {

			if (cropOpEditor.operation !== null) {
				cropOpEditor.operation.width = widthSpinBox.value;
			}
		}

		ToolTip.visible: hovered
		ToolTip.text: qsTr("Cropped image height in pixels")
	}

	Text {
		text: qsTr("Cropped image width :")
	}

	SpinBox {
		id: heightSpinBox

		from: 1
		to: 99999
		editable: true
		value: cropOpEditor.operation.height

		onValueChanged: {

			if (cropOpEditor.operation !== null) {
				cropOpEditor.operation.height = heightSpinBox.value;
			}
		}

		ToolTip.visible: hovered
		ToolTip.text: qsTr("Cropped image height in pixels")
	}

	Text {
		text: qsTr("X offset :")
	}

	SpinBox {
		id: dxSpinBox

		from: -99999
		to: 99999
		editable: true
		value: cropOpEditor.operation.dx

		onValueChanged: {

			if (cropOpEditor.operation !== null) {
				cropOpEditor.operation.dx = dxSpinBox.value;
			}
		}

		ToolTip.visible: hovered
		ToolTip.text: qsTr("Crop box delta x position")
	}

	Text {
		text: qsTr("Y offset :")
	}

	SpinBox {
		id: dySpinBox

		from: -99999
		to: 99999
		editable: true
		value: cropOpEditor.operation.dy

		onValueChanged: {

			if (cropOpEditor.operation !== null) {
				cropOpEditor.operation.dy = dySpinBox.value;
			}
		}

		ToolTip.visible: hovered
		ToolTip.text: qsTr("Crop box delta y position")
	}
	Text {
		text: qsTr("Anchor :")
	}

	ComboBox {
		id: anchorComboBox

		onCurrentTextChanged: {
			operation.anchor = anchorComboBox.currentText
		}

		ToolTip.visible: hovered
		ToolTip.text: qsTr("Anchor of the crop box.")
	}

	Text {
		text: qsTr("Allow extend :")
	}

	Item {
		width: heightSpinBox.width
		height: allowExtendCheckBox.height

		CheckBox {
			id: allowExtendCheckBox
			tristate: false

			anchors.centerIn: parent

			onCheckStateChanged: {
				operation.allowGrowing = allowExtendCheckBox.checkState === Qt.Checked;
			}
		}
	}



	Text {
		text: qsTr("Background color :")
	}

	ColorSelector {
		id: colorSelector

		color: cropOpEditor.operation.bg
		width: heightSpinBox.width

		onColorChanged: {
			cropOpEditor.operation.bg = colorSelector.color;
		}
	}

	onOperationChanged: {

		widthSpinBox.value = operation.width
		heightSpinBox.value = operation.height
		dxSpinBox.value = operation.dx
		dySpinBox.value = operation.dy
		allowExtendCheckBox.checkState = (operation.allowGrowing) ? Qt.Checked : Qt.Unchecked;
		colorSelector.color = operation.bg

		var oldAnchor = operation.anchor

		anchorComboBox.model = operation.possibleAnchors

		var anchorIndex = anchorComboBox.find(oldAnchor)
		if (anchorIndex >= 0) {
			anchorComboBox.currentIndex = anchorIndex
		}
	}

	Connections {
		target: operation
		onWidthChanged: {
			widthSpinBox.value = operation.width
		}
		onHeightChanged: {
			heightSpinBox.value = operation.height
		}
		onDxChanged: {
			dxSpinBox.value = operation.dx
		}
		onDyChanged: {
			dySpinBox.value = operation.dy
		}
		onAllowGrowingChanged: {
			allowExtendCheckBox.checkState = (operation.allowGrowing) ? Qt.Checked : Qt.Unchecked;
		}
		onBgChanged: {
			colorSelector.color = operation.bg
		}

		onAnchorChanged: {
			var anchorIndex = anchorComboBox.find(operation.anchor)
			if (anchorIndex >= 0) {
				anchorComboBox.currentIndex = anchorIndex
			}
		}
	}

}
