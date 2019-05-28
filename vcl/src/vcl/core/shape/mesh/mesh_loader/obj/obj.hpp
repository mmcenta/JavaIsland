#pragma once

#include "../../mesh_structure/mesh.hpp"

namespace vcl
{

mesh mesh_load_file_obj(const std::string& filename);
std::vector<vec3> mesh_load_file_obj_read_vertices(const std::string& filename);
std::vector<index3> mesh_load_file_obj_read_connectivity(const std::string& filename);

}
