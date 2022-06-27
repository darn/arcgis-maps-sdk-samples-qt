// [WriteFile Name=SetMaxExtent, Category=Maps]
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

#ifdef PCH_BUILD
#include "pch.hpp"
#endif // PCH_BUILD

#include "SetMaxExtent.h"

#include "Envelope.h"
#include "GraphicsOverlay.h"
#include "Graphic.h"
#include "Map.h"
#include "MapQuickView.h"
#include "Point.h"
#include "SimpleLineSymbol.h"
#include "SimpleRenderer.h"
#include "SpatialReference.h"
#include "Viewpoint.h"

using namespace Esri::ArcGISRuntime;

SetMaxExtent::SetMaxExtent(QObject* parent /* = nullptr */):
  QObject(parent),
  m_map(new Map(BasemapStyle::ArcGISStreets, this))
{

}

SetMaxExtent::~SetMaxExtent() = default;

void SetMaxExtent::init()
{
  // Register the map view for QML
  qmlRegisterType<MapQuickView>("Esri.Samples", 1, 0, "MapView");
  qmlRegisterType<SetMaxExtent>("Esri.Samples", 1, 0, "SetMaxExtentSample");
}

MapQuickView* SetMaxExtent::mapView() const
{
  return m_mapView;
}

// Set the view (created in QML)
void SetMaxExtent::setMapView(MapQuickView* mapView)
{
  if (!mapView || mapView == m_mapView)
    return;

  m_graphicsOverlay = new GraphicsOverlay(this);
  Point northEastPoint(-12139393.2109, 5012444.0468, SpatialReference::webMercator());
  Point southWestPoint(-11359277.5124, 4438148.7816, SpatialReference::webMercator());
  m_coloradoEnvelope = Envelope(northEastPoint, southWestPoint);

  SimpleLineSymbol* dashLine = new SimpleLineSymbol(SimpleLineSymbolStyle::Dash, QColor("red"), 1.0f, this);
  SimpleRenderer* simpleRenderer = new SimpleRenderer(dashLine, this);
  m_graphicsOverlay->setRenderer(simpleRenderer);

  addPoint(northEastPoint);
  addPoint(southWestPoint);

  m_mapView = mapView;
  m_mapView->setMap(m_map);
  m_mapView->graphicsOverlays()->append(m_graphicsOverlay);

  m_map->setMaxExtent(m_coloradoEnvelope);

  m_mapView->setViewpointGeometry(Envelope(northEastPoint, southWestPoint), 50);

  emit mapViewChanged();
}

void SetMaxExtent::addPoint(Point &point)
{
  // create graphic
  Graphic* graphic = new Graphic(point, this);

  // add graphic to Graphic Overlay
  m_graphicsOverlay->graphics()->append(graphic);
}
