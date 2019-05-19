#include "polygon.hpp"

#include "../../../../../../external/earcut/earcut.hpp"

namespace vcl
{

void polygon_to_curves(std::vector<curve_drawable>& curves, const polygon& poly, const vec3 color)
{
	for(const std::vector<vec3>& ring : poly.ring)
	{
		curves.push_back(ring);
		curves.back().uniform_parameter.color = color;
	}
}

mesh polygon_to_mesh(const polygon& poly)
{
	mesh m;
	for(const std::vector<vec3>& ring : poly.ring)
	{
		m.position.insert(m.position.end(), ring.begin(), ring.end());
	}
	
	unsigned int nv = 0;
	for(unsigned int i=0; i<poly.ring.size(); ++i) 
	{
		std::vector<std::vector<vec3>> ring = {poly.ring[i]};
		std::vector<unsigned int> indices = mapbox::earcut<unsigned int>(ring);
		for(unsigned int i = 0; i < indices.size(); i+=3)
		{
			m.connectivity.push_back({nv+indices[i],nv+indices[i+1],nv+indices[i+2]});
		}
		nv += poly.ring[i].size();
	}
	return m;
}

void polygons_to_curves(std::vector<curve_drawable>& curves, const std::vector<polygon>& polygons, const vec3 color)
{
	for(const polygon& poly : polygons)
	{
		polygon_to_curves(curves, poly, color);
	}
}

mesh polygons_to_mesh(const std::vector<polygon>& polygons)
{
	mesh m;
	for(const polygon& poly : polygons)
	{
		m.push_back(polygon_to_mesh(poly));
	}
	return m;
}

}
