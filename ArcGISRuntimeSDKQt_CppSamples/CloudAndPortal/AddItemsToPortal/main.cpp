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
#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
#include <QtWebEngineQuick>
#endif // QT_WEBVIEW_WEBENGINE_BACKEND

#include "Esri/ArcGISRuntime/Toolkit/register.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "AddItemsToPortal.h"

#define STRINGIZE(x) #x
#define QUOTE(x) STRINGIZE(x)

int main(int argc, char *argv[])
{
  // Enforce OpenGL
  qputenv("QSG_RHI_BACKEND", "opengl");

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
  QtWebEngineQuick::initialize();
#endif // QT_WEBVIEW_WEBENGINE_BACKEND

  QGuiApplication app(argc, argv);
  app.setApplicationName("Add Items to Portal - C++");

  // Initialize the sample
  AddItemsToPortal::init();

  // Initialize application view
  QQuickView view;
  view.setResizeMode(QQuickView::SizeRootObjectToView);

  // Add the import Path
  view.engine()->addImportPath(QDir(QCoreApplication::applicationDirPath()).filePath("qml"));

  QString arcGISRuntimeImportPath = QUOTE(ARCGIS_RUNTIME_IMPORT_PATH);

 #if defined(LINUX_PLATFORM_REPLACEMENT)
  // on some linux platforms the string 'linux' is replaced with 1
  // fix the replacement paths which were created
  QString replaceString = QUOTE(LINUX_PLATFORM_REPLACEMENT);
  arcGISRuntimeImportPath = arcGISRuntimeImportPath.replace(replaceString, "linux", Qt::CaseSensitive);
 #endif

  // Add the Runtime and Extras path
  view.engine()->addImportPath(arcGISRuntimeImportPath);

  Esri::ArcGISRuntime::Toolkit::registerComponents(*(view.engine()));

  // Set the source
  view.setSource(QUrl("qrc:/Samples/CloudAndPortal/AddItemsToPortal/AddItemsToPortal.qml"));

  view.show();

  return app.exec();
}
