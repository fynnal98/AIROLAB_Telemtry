import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 700
    height: 400
    title: "Telemetry AiroLab"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Überschrift
        Label {
            text: "TELEMETRY AIROLAB"
            font.pixelSize: 24
            font.bold: true
            color: "black"
            padding: 10
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }

        // Tabs im Explorer-Stil
        RowLayout {
            id: tabRow
            Layout.fillWidth: true
            spacing: 2

            function selectTab(index) { stack.currentIndex = index }

            Repeater {
                model: [
                    { text: "Telemetrie", icon: "qrc:/icons/plane.png" },
                    { text: "Logs",       icon: "qrc:/icons/log.png" },
                    { text: "Settings",   icon: "qrc:/icons/settings.png" }
                ]

                delegate: Button {
                    checkable: true
                    checked: stack.currentIndex === index
                    onClicked: tabRow.selectTab(index)

                    Layout.preferredWidth: 140
                    Layout.preferredHeight: 35

                    background: Rectangle {
                        color: checked ? "white" : "#e0e0e0"
                        border.color: "#a0a0a0"
                        border.width: 1
                        radius: 3
                    }

                    contentItem: Row {
                        anchors.centerIn: parent
                        spacing: 6

                        Image {
                            source: model.icon
                            width: 18
                            height: 18
                        }

                        Text {
                            text: model.text
                            color: checked ? "black" : "gray"
                            font.bold: checked
                            font.pixelSize: 14
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }

            Item { Layout.fillWidth: true } // Leerraum rechts
        }

        // Linie unter Tabs
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#a0a0a0"
        }

        // Inhalte der Tabs
        StackLayout {
            id: stack
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                color: "white"
                anchors.fill: parent
                Text {
                    text: "Telemetrie-Daten"
                    anchors.centerIn: parent
                    font.pixelSize: 18
                }
            }

            Rectangle {
                color: "white"
                anchors.fill: parent
                Text {
                    text: "Logs hier"
                    anchors.centerIn: parent
                    font.pixelSize: 18
                }
            }

            Rectangle {
                color: "white"
                anchors.fill: parent
                Text {
                    text: "Settings"
                    anchors.centerIn: parent
                    font.pixelSize: 18
                }
            }
        }
    }
}
