// [WriteFile Name=DisplayRouteLayer, Category=Routing]
// [Legal]
// Copyright 2022 Esri.

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// [Legal]

import QtQuick 2.12
import QtQuick.Controls 2.12
import Esri.Samples 1.0
import QtQuick.Layouts 1.11

Item {

    // add a mapView component
    MapView {
        id: view
        anchors.fill: parent

        Component.onCompleted: {
            // Set and keep the focus on SceneView to enable keyboard navigation
            forceActiveFocus();
        }

        Button {
            text: "Open"
            onClicked: popup.open()
            anchors.bottom: parent.bottom
            anchors.margins: 40
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Popup {
            id: popup
            anchors.centerIn: Overlay.overlay
            width: 200
            height: 300
            modal: true
            focus: true
            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent | Popup.CloseOnPressOutside | Popup.CloseOnReleaseOutsideParent
            contentItem: Text {
//                text: model.getDirections();
            }
        }
    }

    // Declare the C++ instance which creates the scene etc. and supply the view
    DisplayRouteLayerSample {
        id: model
        mapView: view
    }
}
