import QtQuick 2.0

Text {
    text: parent.text
    font: parent.font
    color: parent.checked ? "white" : "#f4a649"
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    elide: Text.ElideRight
}
