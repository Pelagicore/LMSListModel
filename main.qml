import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

import LMSListModel 1.0

ApplicationWindow {
    id: applicationWindow1
    title: qsTr("Hello World")
    width: 800
    height: 1000
    visible: true

    Item {
        id: tracksItem
        width: parent.width
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        ListView {
            id: tracksList
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: selectorItem.bottom
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0

            model: LMSListModel {
                id: mainListModel
                database: "/mnt/usb/jlr/sample_music/music.db"
            }

            delegate: Item {
                height: 100
                width: parent.width
                Text{
                    id: titleText
                    anchors.top: parent.top
                    anchors.left: parent.left
                    text: titleColumn
                }
                Text{
                    id: artistText
                    anchors.top: titleText.bottom
                    anchors.left: parent.left
                    text: artistColumn
                }

                Text{
                    id: albumText
                    anchors.top: artistText.bottom
                    anchors.left: parent.left
                    text: albumColumn
                }

                Text{
                    id: pathText
                    anchors.top: albumText.bottom
                    anchors.left: parent.left
                    text: pathColumn
                }

                Image {
                    source: "file://" + albumArtColumn
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: height
                }
            }
        }

        Item {
            height: 30
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            id: selectorItem

            ExclusiveGroup {id: queryGroup}

            RadioButton {
                id: artistsRadioButton
                text: qsTr("Artists")
                anchors.left: parent.left
                anchors.leftMargin: 0
                exclusiveGroup: queryGroup
                onCheckedChanged: {
                    if (checked) {
                        mainListModel.queryType = LMSListModel.Artists
                    }
                }
            }

            RadioButton {
                id: albumsRadioButton
                text: qsTr("Albums")
                anchors.left: artistsRadioButton.right
                anchors.leftMargin: 0
                exclusiveGroup: queryGroup
                onCheckedChanged: {
                    if (checked) {
                        mainListModel.queryType = LMSListModel.Albums
                    }
                }
            }

            RadioButton {
                id: videosRadioButton
                text: qsTr("Videos")
                anchors.left: albumsRadioButton.right
                anchors.leftMargin: 0
                exclusiveGroup: queryGroup
                onCheckedChanged: {
                    if (checked) {
                        mainListModel.queryType = LMSListModel.Videos
                    }
                }
            }

            CheckBox {
                id: adeleCheckbox
                text: qsTr("I only like Adele!")
                anchors.left: tracksRadioButton.right
                anchors.leftMargin: 0
                onCheckedChanged: {
                    if (checked) {
                        mainListModel.artist = "Adele"
                    } else {
                        mainListModel.artist = ""
                    }
                }
            }

            TextInput {
                id: trackTitleQuery
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: adeleCheckbox.right
                anchors.leftMargin: 0
                font.pixelSize: 12
                onTextChanged: {
                    mainListModel.track = text
                }
            }

            RadioButton {
                id: tracksRadioButton
                text: qsTr("Tracks")
                anchors.left: albumsRadioButton.right
                anchors.leftMargin: 0
                exclusiveGroup: queryGroup
                onCheckedChanged: {
                    if (checked) {
                        mainListModel.queryType = LMSListModel.Tracks
                    }
                }
            }



        }
    }
}
