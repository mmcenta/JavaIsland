#include "shp.hpp"

#include "../../../../../../../external/shapelib/shapefil.h"
#include "../../../../helper/file/file.hpp"

namespace vcl
{

std::vector<polygon> polygon_load_file_shp(const std::string& filename)
{
    assert_file_exist(filename);
    
    SHPHandle shp = SHPOpen( filename.c_str(), "rb" );
    assert( shp != NULL );
    
    int		nShapeType, nEntities;
    double 	adfMinBound[4], adfMaxBound[4];
    SHPGetInfo( shp, &nEntities, &nShapeType, adfMinBound, adfMaxBound );

    assert( nShapeType != 1 ); // points
    
    std::cout << "Reading " << filename << " (type=" << nShapeType << ") ... " << std::flush;

    std::vector<polygon> polygons;
    for( int i = 0; i < nEntities; i++ )
    {
        SHPObject	*obj = SHPReadObject( shp, i );
	polygon poly;
	poly.ring.resize(obj->nParts);
    	for (int j = 0; j < obj->nParts; j++) {
		int start = obj->panPartStart[j];
		int end   = (j+1 == obj->nParts) ? obj->nVertices : obj->panPartStart[j+1];
		for(int k = start; k < end; ++k )
		{
		    poly.ring[j].emplace_back(obj->padfX[k], obj->padfY[k], obj->padfZ[k]);
		}
	}
        SHPDestroyObject( obj );
        polygons.push_back( poly );
    }
    
    SHPClose( shp );
    
    std::cout << "OK" << std::endl;

    return polygons;
}

std::vector<vec3> points_load_file_shp(const std::string& filename)
{
    assert_file_exist(filename);
    
    SHPHandle shp = SHPOpen( filename.c_str(), "rb" );
    assert( shp != NULL );
    
    int		nShapeType, nEntities;
    double 	adfMinBound[4], adfMaxBound[4];
    SHPGetInfo( shp, &nEntities, &nShapeType, adfMinBound, adfMaxBound );

    assert( nShapeType == 1 ); // points
    
    std::cout << "Reading " << filename << " (type=" << nShapeType << ") ... " << std::flush;

    std::vector<vec3> points;
    for( int i = 0; i < nEntities; i++ )
    {
        SHPObject	*obj = SHPReadObject( shp, i );
        points.emplace_back( obj->padfX[0], obj->padfY[0], obj->padfZ[0] );
        SHPDestroyObject( obj );
    }
    
    SHPClose( shp );
    
    std::cout << "OK" << std::endl;

    return points;
}

}
