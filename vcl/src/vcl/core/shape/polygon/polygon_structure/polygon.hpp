#pragma once

#include "../../../math/vec/vec.hpp"
#include "../../../drawable/curve_drawable/curve_drawable.hpp"
#include "../../../shape/mesh/mesh.hpp"

#include <vector>

namespace vcl
{

/** A polygon with hole structure
*/
struct polygon
{
    /** collection of polygonal rings. ring[0] is the outer ring */
    std::vector<std::vector<vec3>> ring;
};

void polygon_to_curves(std::vector<curve_drawable>& curves, const polygon& poly, const vec3 color);
void polygons_to_curves(std::vector<curve_drawable>& curves, const std::vector<polygon>& polygons, const vec3 color);

vcl::mesh polygon_to_mesh(const polygon& poly);
vcl::mesh polygons_to_mesh(const std::vector<polygon>& polygons);

}
