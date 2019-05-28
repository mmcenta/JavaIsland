#pragma once

#ifdef __linux__
#pragma GCC diagnostic ignored "-Weffc++"
#endif 

#include "../../../math/vec/vec.hpp"


#include <vector>
#include <array>

namespace vcl
{

/** An alias to designate as 'index3' the triplet of integer used to define a triangle index connectivity */
using index3 = std::array<unsigned int, 3>;

/** A mesh structure stores the per-vertex informations as well as the triangle connectivity.
 *  All buffers are stored contiguously in the CPU memory.
*/
struct mesh
{
    /** Per-vertex attributes */
    std::vector<vec3> position;
    std::vector<vec3> normal;
    std::vector<vec4> color;
    std::vector<vec2> texture_uv;

    /** Triangle connectivity */
    std::vector<index3> connectivity;

    /** Fill all per-vertex attributes with default values if they are empty (ex. color to white, and 0 for texture-uv)*/
    void fill_empty_fields();
    /** Add a mesh structure to the current one (concatenate per-vertex attributes, and add triangle indices accordingly)*/
    void push_back(const mesh& mesh_to_add);

    /** Fill per-vertex color value with a constant value */
    void fill_color_uniform(const vec3& c);
    void fill_color_uniform(const vec4& c);
};

/** Compute per-vertex normals given a set of position coordinates and triangle index connectivity */
std::vector<vec3> normal(const std::vector<vec3>& position, const std::vector<index3>& connectivity);

/** Compute per-vertex normals given a set of position coordinates and triangle index connectivity
    Avoid re-allocation of normal vector if it is not needed */
void normal(const std::vector<vec3>& position, const std::vector<index3>& connectivity, std::vector<vec3>& normals);



}
