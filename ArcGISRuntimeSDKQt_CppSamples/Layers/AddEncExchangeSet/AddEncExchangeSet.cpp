// [WriteFile Name=AddEncExchangeSet, Category=Layers]
// [Legal]
// Copyright 2019 Esri.

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

#include "AddEncExchangeSet.h"

#include "Map.h"
#include "MapQuickView.h"
#include "EncExchangeSet.h"
#include "EncLayer.h"
#include "EncDataset.h"
#include "EncCell.h"
#include "EncEnvironmentSettings.h"
#include "Envelope.h"
#include "GeometryEngine.h"
#include "Error.h"
#include "MapTypes.h"
#include "TaskWatcher.h"
#include "LayerListModel.h"
#include "Viewpoint.h"

#include <QtCore/qglobal.h>
#include <QStandardPaths>

using namespace Esri::ArcGISRuntime;

// helper method to get cross platform data path
namespace
{
  QString defaultDataPath()
  {
    QString dataPath;

  #ifdef Q_OS_IOS
    dataPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
  #else
    dataPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  #endif

    return dataPath;
  }
} // namespace

AddEncExchangeSet::AddEncExchangeSet(QObject* parent /* = nullptr */):
  QObject(parent),
  m_map(new Map(BasemapStyle::ArcGISOceans, this))
{
  // set resource path
  EncEnvironmentSettings::setResourcePath(defaultDataPath() + "/ArcGIS/Runtime/Data/ENC/hydrography");

  // create ENC Exchange Set
  QStringList encPaths = { defaultDataPath() + "/ArcGIS/Runtime/Data/ENC/ExchangeSetwithoutUpdates/ENC_ROOT/CATALOG.031" };
  m_encExchangeSet = new EncExchangeSet(encPaths, this);
}

AddEncExchangeSet::~AddEncExchangeSet() = default;

void AddEncExchangeSet::init()
{
  // Register the map view for QML
  qmlRegisterType<MapQuickView>("Esri.Samples", 1, 0, "MapView");
  qmlRegisterType<AddEncExchangeSet>("Esri.Samples", 1, 0, "AddEncExchangeSetSample");
}

MapQuickView* AddEncExchangeSet::mapView() const
{
  return m_mapView;
}

// Set the view (created in QML)
void AddEncExchangeSet::setMapView(MapQuickView* mapView)
{
  if (!mapView || mapView == m_mapView)
    return;

  m_mapView = mapView;
  m_mapView->setMap(m_map);

  // connect to doneLoading signal of EncExchangeSet
  connect(m_encExchangeSet, &EncExchangeSet::doneLoading, this, [this](const Error& e)
  {
    if (!e.isEmpty())
      return;

    // loop through datasets
    const auto datasets = m_encExchangeSet->datasets();
    for (EncDataset* dataset : datasets)
    {
      // create an EncCell from each dataset
      EncCell* cell = new EncCell(dataset, this);

      // create an EncLayer from each cell
      EncLayer* encLayer = new EncLayer(cell, this);

      // connect to the doneLoading signal of the layer
      connect(encLayer, &EncLayer::doneLoading, this, [this, encLayer](const Error& e)
      {
        if (!e.isEmpty())
          return;

        m_extents << encLayer->fullExtent();

        if (m_extents.length() != m_encExchangeSet->datasets().length())
          return;

        // combine the extents
        Envelope fullExtent = GeometryEngine::combineExtents(m_extents);
        m_mapView->setViewpoint(Viewpoint{fullExtent});
      });

      // add the layer to the map
      m_map->operationalLayers()->append(encLayer);
    }
  });

  // load the EncExchangeSet
  m_encExchangeSet->load();

  emit mapViewChanged();
}
