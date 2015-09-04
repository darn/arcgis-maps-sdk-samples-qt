// Copyright 2015 Esri.

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

import QtQuick 2.3
import QtGraphicalEffects 1.0
import Esri.ArcGISRuntime 100.00
import Esri.ArcGISExtras 1.1

Rectangle {
    width: 800
    height: 600

    property real scaleFactor: System.displayScaleFactor

    // Create a MapView
    MapView {
        id: mapView
        anchors.fill: parent
        // Create an initial Map with the Imagery basemap
        Map {
            BasemapImagery {}
        }
    }

    // Create a list model with information about different portal items
    ListModel {
        id: portalItemListModel
        ListElement { itemTitle: "Housing with Mortgages"; imageUrl: "qrc:/Samples/Maps/OpenExistingMap/Housing.png"; itemId: "2d6fa24b357d427f9c737774e7b0f977"}
        ListElement { itemTitle: "USA Tapestry Segmentation"; imageUrl: "qrc:/Samples/Maps/OpenExistingMap/Tapestry.png"; itemId: "01f052c8995e4b9e889d73c3e210ebe3"}
        ListElement { itemTitle: "Geology of United States"; imageUrl: "qrc:/Samples/Maps/OpenExistingMap/geology.jpg"; itemId: "74a8f6645ab44c4f82d537f1aa0e375d"}
    }

    // Create a delegate for how the portal items display in the view
    Component {
        id: portalItemDelegate
        Item {
            width: parent.width
            height: 65 * scaleFactor

            Row {
                spacing: 10
                Image {
                    source: imageUrl
                    width: 100 * scaleFactor
                    height: 65 * scaleFactor
                }
                Row {
                    anchors.verticalCenter: parent.verticalCenter
                    Text {
                        width: 100 * scaleFactor
                        text: itemTitle
                        wrapMode: Text.WordWrap
                    }
                }
            }
            MouseArea {
                anchors.fill: parent
                // When an item in the list view is clicked
                onClicked: {
                    portalItemListView.currentIndex = index
                    // Create a new, blank map
                    var newMap = ArcGISRuntimeEnvironment.createObject("Map");
                    // Create a PortalItem and assign it a URL and item ID
                    var newPortalItem = ArcGISRuntimeEnvironment.createObject("PortalItem", {url:"http://arcgis.com"});
                    newPortalItem.itemId = itemId;
                    // Set the portalItem property on the Map
                    newMap.portalItem = newPortalItem;
                    // Set the map to the MapView
                    mapView.map = newMap;
                    mapPickerWindow.visible = false;
                }
            }
        }
    }

    // Create a window to display the different Portal Items that can be selected
    Rectangle {
        id: mapPickerWindow
        anchors.fill: parent
        color: "transparent"

        GaussianBlur {
            anchors.fill: mapPickerWindow
            source: mapView
            radius: 40
            samples: 20
        }

        MouseArea {
            anchors.fill: parent
            onClicked: mouse.accepted = true
            onWheel: wheel.accepted = true
        }

        Rectangle {
            anchors.centerIn: parent
            width: 250 * scaleFactor
            height: 200 * scaleFactor
            color: "lightgrey"
            opacity: .8
            radius: 5
            border {
                color: "#4D4D4D"
                width: 1
            }

            // Create a list view to display the items
            ListView {
                id: portalItemListView
                anchors {
                    fill: parent
                    margins: 10 * scaleFactor
                }
                // Assign the model to the list model of portal items
                model: portalItemListModel
                // Assign the delegate to the delegate created above
                delegate: portalItemDelegate
                spacing: 10
                clip: true
                highlightFollowsCurrentItem: true
                highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
                focus: true
            }
        }
    }

    // Create a button to show the map picker window
    Rectangle {
        id: switchButton
        visible: !mapPickerWindow.visible
        anchors {
            right: parent.right
            bottom: parent.bottom
            rightMargin: 20 * scaleFactor
            bottomMargin: 40 * scaleFactor
        }

        width: 45 * scaleFactor
        height: width
        color: "#D6D6D6"
        radius: 100
        layer.enabled: true
        layer.effect: Glow {
            radius: 1.5
            samples: radius * 2
            source: switchButton
            color: "#585858"
            transparentBorder: true
        }

        Image {
            anchors.centerIn: parent
            width: 35 * scaleFactor
            height: width
            source: "qrc:/Samples/Maps/OpenExistingMap/SwitchMap.png"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                // Show the add window when it is clicked
                mapPickerWindow.visible = true;
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border {
            width: 0.5 * scaleFactor
            color: "black"
        }
    }
}
