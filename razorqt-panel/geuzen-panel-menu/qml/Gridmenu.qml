import QtQuick 1.0

Rectangle {
  id: mainBox
  width: 300; height: 300
  color: "lightblue"
  Text {
    anchors.centerIn: parent
    text: "No Grid Menu Implementation"
  }
  MouseArea {
    anchors.fill: parent
    onClicked: {
      Qt.quit ()
    }
  }
}
