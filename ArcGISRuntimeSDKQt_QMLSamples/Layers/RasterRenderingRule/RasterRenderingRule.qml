// [WriteFile Name=RasterRenderingRule, Category=Layers]
// [Legal]
// Copyright 2017 Esri.

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

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Esri.ArcGISRuntime
import Esri.ArcGISExtras

Rectangle {
    id: rootRectangle
    clip: true
    width: 800
    height: 600

    property var renderingRuleNames: []
    readonly property url imageServiceUrl: "https://sampleserver6.arcgisonline.com/arcgis/rest/services/CharlotteLAS/ImageServer"

    MapView {
        id: mapView
        anchors.fill: parent

        Component.onCompleted: {
            // Set the focus on MapView to initially enable keyboard navigation
            forceActiveFocus();
        }

        Map {
            id: map
            // create a basemap from a tiled layer and add to the map
            Basemap {
                initStyle: Enums.BasemapStyleArcGISStreets
            }

            //! [RasterRenderingRule qml ImageServiceRaster]
            // create and add a raster layer to the map
            RasterLayer {
                // create the raster layer from an image service raster
                ImageServiceRaster {
                    id: imageServiceRaster
                    url: imageServiceUrl

                    // zoom to the extent of the raster once it's loaded
                    onLoadStatusChanged: {
                        if (loadStatus === Enums.LoadStatusLoaded) {
                            mapView.setViewpointGeometry(imageServiceRaster.serviceInfo.fullExtent);

                            const renderingRuleInfos = imageServiceRaster.serviceInfo.renderingRuleInfos;
                            const names = [];
                            for (let i = 0; i < renderingRuleInfos.length; i++) {
                                names.push(renderingRuleInfos[i].name);
                            }
                            renderingRuleNames = names;
                        }
                    }
                }
            }
            //! [RasterRenderingRule qml ImageServiceRaster]
        }

        Rectangle {
            anchors {
                left: parent.left
                top: parent.top
                margins: 5
            }
            height: childrenRect.height
            width: childrenRect.width
            color: "silver"
            radius: 5

            GridLayout {
                columns: 2

                Label {
                    Layout.margins: 10
                    Layout.columnSpan: 2
                    Layout.alignment: Qt.AlignHCenter
                    text: "Apply a Rendering Rule"
                    font.pixelSize: 16
                }

                ComboBox {
                    id: renderingRulesCombo
                    property int modelWidth: 0
                    Layout.minimumWidth: modelWidth + leftPadding + rightPadding
                    model: renderingRuleNames
                    onModelChanged: {
                        for (let i = 0; i < model.length; ++i) {
                            metrics.text = model[i];
                            modelWidth = Math.max(modelWidth, metrics.width);
                        }
                    }
                    TextMetrics {
                        id: metrics
                        font: renderingRulesCombo.font
                    }
                }

                Button {
                    id: applyButton
                    Layout.margins: 10
                    text: "Apply"
                    onClicked: {
                        applyRenderingRule(renderingRulesCombo.currentIndex);
                    }
                }
            }
        }
    }

    function applyRenderingRule(index) {
        //! [ImageServiceRaster Create a rendering rule]
        // get the rendering rule info at the selected index
        const renderingRuleInfo = imageServiceRaster.serviceInfo.renderingRuleInfos[index];
        // create a rendering rule object using the rendering rule info
        const renderingRule = ArcGISRuntimeEnvironment.createObject("RenderingRule", {renderingRuleInfo: renderingRuleInfo});
        // create a new image service raster
        const newImageServiceRaster = ArcGISRuntimeEnvironment.createObject("ImageServiceRaster", {url: imageServiceUrl});
        // apply the rendering rule
        newImageServiceRaster.renderingRule = renderingRule;
        // create a raster layer using the image service raster
        const rasterLayer = ArcGISRuntimeEnvironment.createObject("RasterLayer", {raster: newImageServiceRaster});
        //! [ImageServiceRaster Create a rendering rule]
        // add the raster layer to the map
        map.operationalLayers.append(rasterLayer);
    }
}
