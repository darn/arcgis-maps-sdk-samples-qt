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

#include <QGuiApplication>
#include <QQuickView>
#include <QCommandLineParser>
#include <QDir>
#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QSurfaceFormat>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "MapQuickView.h"
#include "SceneQuickView.h"
#include "ArcGISRuntimeEnvironment.h"

#include "Animate3DSymbols.h"

#define STRINGIZE(x) #x
#define QUOTE(x) STRINGIZE(x)

using namespace Esri::ArcGISRuntime;

int main(int argc, char *argv[])
{
  // Enforce OpenGL
  qputenv("QSG_RHI_BACKEND", "opengl");

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
  // Linux requires 3.2 OpenGL Context
  // in order to instance 3D symbols
  QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
  fmt.setVersion(3, 2);
  QSurfaceFormat::setDefaultFormat(fmt);
#endif

  QGuiApplication app(argc, argv);
  app.setApplicationName("Animate3DSymbols - C++");

  // Use of Esri location services, including basemaps and geocoding,
  // requires authentication using either an ArcGIS identity or an API Key.
  // 1. ArcGIS identity: An ArcGIS named user account that is a member of an
  //    organization in ArcGIS Online or ArcGIS Enterprise.
  // 2. API key: A permanent key that gives your application access to Esri
  //    location services. Visit your ArcGIS Developers Dashboard create a new
  //    API keys or access an existing API key.
  const QString apiKey = QString("");
  if (apiKey.isEmpty())
  {
      qWarning() << "Use of Esri location services, including basemaps, requires" <<
                    "you to authenticate with an ArcGIS identity or set the API Key property.";
  }
  else
  {
      Esri::ArcGISRuntime::ArcGISRuntimeEnvironment::setApiKey(apiKey);
  }

  // Initialize the sample
  Animate3DSymbols::init();

  // Initialize application view
  QQmlApplicationEngine engine;

  // Add the import Path
  engine.addImportPath(QDir(QCoreApplication::applicationDirPath()).filePath("qml"));

  QString arcGISRuntimeImportPath = QUOTE(ARCGIS_RUNTIME_IMPORT_PATH);

 #if defined(LINUX_PLATFORM_REPLACEMENT)
  // on some linux platforms the string 'linux' is replaced with 1
  // fix the replacement paths which were created
  QString replaceString = QUOTE(LINUX_PLATFORM_REPLACEMENT);
  arcGISRuntimeImportPath = arcGISRuntimeImportPath.replace(replaceString, "linux", Qt::CaseSensitive);
 #endif

  // Add the Runtime and Extras path
  engine.addImportPath(arcGISRuntimeImportPath);

  // Set the source
  engine.load(QUrl("qrc:/Samples/Scenes/Animate3DSymbols/main.qml"));

  return app.exec();
}
