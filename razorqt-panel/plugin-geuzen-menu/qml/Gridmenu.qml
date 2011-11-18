import QtQuick 1.0

Rectangle {
  id: mainBox
  width: 600; height: 400
  clip: true
  color: "#f2f1ef"
  property real gridCellWidth: 200
  property real gridCellHeight: 50
  property real itemWidth: 196
  property real itemHeight: 48
  property real iconWidth: 32
  property real iconHeight: 32
  property real labelWidth: itemWidth - iconWidth - 2
  property alias menuTitle: titleText.text

  signal cancelled ()
  signal selected (int kind, int tag)

  Text {
    id: titleText
    anchors {
      top: mainBox.top; horizontalCenter: mainBox.horizontalCenter 
    }
    text: "Main Menu"
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
    clip: true
    cellWidth: mainBox.gridCellWidth
    cellHeight: mainBox.gridCellHeight
    height: mainBox.height - titleText.height
    anchors {
      top: titleText.bottom; left: mainBox.left
    }
    delegate: Rectangle {
      width: mainBox.itemWidth
      height: mainBox.itemHeight
      color: "#f0f0f7"
      border.color: "#f7f7ff"; border.width: 2
      radius: 6
      Row {
        spacing: 2
        anchors.centerIn: parent
        height: mainBox.itemHeight
        Image {
          width: mainBox.iconWidth
          height: mainBox.iconHeight
          source: iconName
        }
        Rectangle {
          width: mainBox.labelWidth; height: mainBox.itemHeight
          color: "transparent"
          Text {
            id: itemText
            elide: Text.ElideRight
            width: parent.width
            anchors.verticalCenter: parent.verticalCenter
            text: (isMenu ? "M " : "") + itemTitle 
          }
        }
      }
      MouseArea {
        anchors.fill: parent
        onClicked: {
          mainBox.selected (itemKind, itemTag)
        }
      }
    }
  }
  Component.onCompleted: {
    console.log ("loaded main box")
  }
}
