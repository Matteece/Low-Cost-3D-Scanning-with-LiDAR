close all;

shp = alphaShape(x(:),y(:),z(:), 'HoleThreshold',10);
[bf, P] = boundaryFacets(shp);
stlwrite(triangulation(bf, P), 'LivingRoom.stl');