# Marine Chart TileServer

An HTTP tile server with integrated control plane supporting ingest of S57 and BSB4 marine chart formats and egest of
popular raster/vector web map format(s).


### Feature Complete

- Control plane API
    - none complete
    
- Public API
    - none complete


### Features WIP

- Control plane API
    - upsert world base chart POST `tbd`
    
    - upsert BSB4 chart POST `tbd`
    - delete BSB4 chart DELETE `tbd` 
    
    - upsert GeoTiff chart POST `tbd`
    - delete GeoTiff chart DELETE `tbd`
    
    - upsert S57 chart POST `tbd`
    - delete S57 chart DELETE `tbd`

- Public API
    - raster region tiles GET `/v1/raster/mbtiles`
    - raster png tiles GET `/v1/png/{z}/{x}/{y}`
    
    - vector region tiles GET `/v1/vector/mbtiles`
    - vector tiles GET `/v1/vec/{z}/{x}/{y}`
    
    
### Todo:

- dependencies 
    - ~~proxygen~~
    - geometry
    - vector-tile
    - rapidjson
    - protozero
    - gdal
    - geos
    - abseil (?)
    
- containerize
