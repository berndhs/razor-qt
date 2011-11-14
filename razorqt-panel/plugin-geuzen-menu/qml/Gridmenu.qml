import QtQuick 1.0

Rectangle {
  id: mainBox
  width: 600; height: 400
  color: "lightblue"
  opacity: 0.75
  property real gridCellWidth: 100
  property real gridCellHeight: 50
  property real itemWidth: 92
  property real itemHeight: 48
  signal cancelled ()
  Text {
    id: titleText
    anchors {
      top: mainBox.top; horizontalCenter: mainBox.horizontalCenter 
    }
    text: "No Grid Menu Implementation"
    MouseArea {
      anchors.fill: parent
      onClicked: {
        mainBox.cancelled ()
      }
    }
  }
  GridView {
    id: menuGrid
    model: cppMenuModel
    width: mainBox.width
    cellWidth: mainBox.gridCellWidth
    cellHeight: mainBox.gridCellHeight
    height: mainBox.height - titleText.height
    anchors {
      top: titleText.bottom; left: mainBox.left
    }
    delegate: Rectangle {
      width: mainBox.itemWidth
      height: mainBox.itemHeight
      color: Qt.lighter (mainBox.color)
      radius: 6
      Text {
        id: itemText
        anchors.centerIn: parent
        elide: Text.ElideRight
        width: parent.width
        text: (isMenu ? "M " : "A ") + title 
      }
    }
  }
}
