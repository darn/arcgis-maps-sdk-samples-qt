// [WriteFile Name=StyleWmsLayer, Category=Layers]
// [Legal]
// Copyright 2018 Esri.

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

#ifndef STYLEWMSLAYER_H
#define STYLEWMSLAYER_H

namespace Esri::ArcGISRuntime
{
class Map;
class MapQuickView;
class WmsSublayer;
}

#include <QQuickItem>

class StyleWmsLayer : public QQuickItem
{
  Q_OBJECT

public:
  explicit StyleWmsLayer(QQuickItem* parent = nullptr);
  ~StyleWmsLayer() override = default;

  void componentComplete() override;
  static void init();
  Q_INVOKABLE void setCurrentStyle(int index);

private:
  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  Esri::ArcGISRuntime::WmsSublayer* m_wmsSublayer = nullptr;
};

#endif // STYLEWMSLAYER_H
