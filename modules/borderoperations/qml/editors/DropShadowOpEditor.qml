import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

import "qrc:/qmlcomponents/controls/" as PiwapControls

GridLayout {

    property QtObject operation: null
    property int controlsMargin: 5

    id : dropshadowOpEditor

    anchors.left: parent.left
    anchors.right: parent.right

    columns: 2

    columnSpacing: 5
    rowSpacing: 5

    height: childrenRect.height


    Text {
        text: qsTr("x offset :")
    }

    SpinBox {
        id: dxSelector
        from: -999
        to: 999
        editable: true

        onValueChanged: {
            if (dropshadowOpEditor.operation != null) {
                dropshadowOpEditor.operation.dx = dxSelector.value
            }
        }
    }


    Text {
        text: qsTr("y offset :")
    }

    SpinBox {
        id: dySelector
        from: -999
        to: 999
        editable: true

        onValueChanged: {
            if (dropshadowOpEditor.operation != null) {
                dropshadowOpEditor.operation.dy = dySelector.value
            }
        }
    }


    Text {
        text: qsTr("Blur radius :")
    }

    SpinBox {
        id: radiusSelector
        from: 0
        to: 99999
        stepSize: 100
        editable: true

        property int decimals: 2
        property real realValue: value / 100

        validator: DoubleValidator {
            bottom: Math.min(radiusSelector.from, radiusSelector.to)
            top:  Math.max(radiusSelector.from, radiusSelector.to)
        }

        textFromValue: function(value, locale) {
            return Number(value / 100).toLocaleString(locale, 'f', radiusSelector.decimals)
        }

        valueFromText: function(text, locale) {
            return Number.fromLocaleString(locale, text) * 100
        }

        onValueChanged: {
            if (dropshadowOpEditor.operation != null) {
                dropshadowOpEditor.operation.blurRadius = radiusSelector.value/100
            }
        }
    }

    Text {
        text: qsTr("Background color :")
    }

    PiwapControls.ColorSelector {
        id: colorSelector

        color: dropshadowOpEditor.operation.bg

        onColorChanged: {
            if (dropshadowOpEditor.operation != null) {
                dropshadowOpEditor.operation.bg = colorSelector.color;
            }
        }
    }

    onOperationChanged: {

        dxSelector.value = dropshadowOpEditor.operation.dx
        dySelector.value = dropshadowOpEditor.operation.dy

        radiusSelector.value = operation.blurRadius*100

        colorSelector.color = operation.bg
    }

    Connections {
        target: operation

        onBlurRadius_changed: {
            radiusSelector.value = operation.blurRadius*100
        }

        onBg_changed: {
            colorSelector.color = operation.bg
        }

        onDx_changed: {
            dxSelector.value = dropshadowOpEditor.operation.dx
        }
        onDy_changed: {
            dySelector.value = dropshadowOpEditor.operation.dy
        }
    }

}
