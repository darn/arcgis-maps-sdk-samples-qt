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

#include <QGuiApplication>
#include <QQuickView>
#include <QCommandLineParser>
#include <QDir>
#include <QQmlEngine>

#include "ArcGISRuntimeEnvironment.h"
#include "Esri/ArcGISRuntime/Toolkit/register.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "GenerateOfflineMap_Overrides.h"

#define STRINGIZE(x) #x
#define QUOTE(x) STRINGIZE(x)

int main(int argc, char *argv[])
{
  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);
  app.setApplicationName("GenerateOfflineMap_Overrides - C++");

  // Use of Esri location services, including basemaps and geocoding,
  // requires authentication using either an ArcGIS identity or an API Key.
  // 1. ArcGIS identity: An ArcGIS named user account that is a member of an
  //    organization in ArcGIS Online or ArcGIS Enterprise.
  // 2. API key: A permanent key that gives your application access to Esri
  //    location services. Visit your ArcGIS Developers Dashboard create a new
  //    API keys or access an existing API key.
//  const QString apiKey = QString("");

  QCommandLineOption apiKeyOption("apikey", "Enter API Key", "apiKeyOption");
  QCommandLineOption licenseOption(QStringList() << "licenselevel" << "l", QCoreApplication::translate("main", "the level of license"), QCoreApplication::translate("main","license"));

  QCommandLineParser commandLineParser;
  commandLineParser.setApplicationDescription("Sample app");
  commandLineParser.addOption(apiKeyOption);
  commandLineParser.addOption(licenseOption);
  commandLineParser.addHelpOption();
  commandLineParser.process(app);

  const auto apiKeyString = commandLineParser.value(apiKeyOption);
  if (!apiKeyString.isEmpty())
  {
    qDebug() << "setting API Key";
    Esri::ArcGISRuntime::ArcGISRuntimeEnvironment::setApiKey(apiKeyString);
  }
  else
  {
      qWarning() << "Use of Esri location services, including basemaps, requires" <<
                    "you to authenticate with an ArcGIS identity or set the API Key property.";
  }

  const auto licenseString = commandLineParser.value(licenseOption);
  if (!licenseString.isEmpty())
  {
    qDebug() << "setting license";
    Esri::ArcGISRuntime::ArcGISRuntimeEnvironment::setLicense(licenseString);
  }


  // Initialize the sample
  GenerateOfflineMap_Overrides::init();

  // Initialize application view
  QQuickView view;
  view.setResizeMode(QQuickView::SizeRootObjectToView);

  QString arcGISRuntimeImportPath = QUOTE(ARCGIS_RUNTIME_IMPORT_PATH);

  Esri::ArcGISRuntime::Toolkit::registerComponents(*(view.engine()));

#if defined(LINUX_PLATFORM_REPLACEMENT)
  // on some linux platforms the string 'linux' is replaced with 1
  // fix the replacement paths which were created
  QString replaceString = QUOTE(LINUX_PLATFORM_REPLACEMENT);
  arcGISRuntimeImportPath = arcGISRuntimeImportPath.replace(replaceString, "linux", Qt::CaseSensitive);
#endif

  // Add the import Path
  view.engine()->addImportPath(QDir(QCoreApplication::applicationDirPath()).filePath("qml"));
  // Add the Runtime and Extras path
  view.engine()->addImportPath(arcGISRuntimeImportPath);

  // Set the source
  view.setSource(QUrl("qrc:/Samples/Maps/GenerateOfflineMap_Overrides/GenerateOfflineMap_Overrides.qml"));

  view.show();

  return app.exec();
}
