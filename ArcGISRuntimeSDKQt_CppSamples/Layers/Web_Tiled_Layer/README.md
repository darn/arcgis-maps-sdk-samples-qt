# Web tiled layer

Display a tiled web layer.

![](screenshot.png)

## Use case

Tiled map services are a set of pre-generated images (e.g. "tiles") arranged in folders for each row, column, and zoom level. As you navigate the map, map tiles are requested for the current extent. `ArcGISTiledLayer` and `WmtsLayer` are types of tiled map services used for specific data types. `WebTiledLayer` is useful for displaying other data sources that contain tiles arranged in a row/column/level directory structure, such as OpenStreetMap.

## How to use the sample

Run the sample and a map will appear. As you navigate the map, map tiles will be fetched automatically and displayed on the map.

## How it works

Web tiled services use a uniform addressing scheme with pre-rendered tiles. Image tiles are accessed via a URL template string, with parameters for subdomain, level, column, and row.
* Subdomain is optional and allows the ArcGIS Maps SDK for Native Apps to balance requests among multiple servers for enhanced performance.
* Level, row, and column select the tiles to load based on the visible extent of the map.

1. Create a `WebTiledLayer` from a URL and a list of subdomains.
2. Create a new `Basemap` from the layer.
3. Update the attribution on the layer. Note: this is a necessary step because web tiled services don't have associated service metadata.
4. Display the basemap.

For more information about web tiled layers, see the following resources:
* [Wikipedia: tiled web maps](https://en.wikipedia.org/wiki/Tiled_web_map)
* [ArcGIS Pro: Share a web tile layer](https://pro.arcgis.com/en/pro-app/help/sharing/overview/web-tile-layer.htm)

## Relevant API

* Basemap
* WebTiledLayer

## About the data

The basemap in this sample is provided by [Stamen Design](maps.stamen.com). Stamen publishes tiled services based on OpenStreetMap data with several unique styles applied.

## Tags

layer, OGC, Open Street Map, OpenStreetMap, stamen.com, tiled, tiles
