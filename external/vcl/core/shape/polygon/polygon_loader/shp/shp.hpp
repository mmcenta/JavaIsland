#pragma once

#include "../../polygon_structure/polygon.hpp"

namespace vcl
{

std::vector<polygon> polygon_load_file_shp(const std::string& filename);
std::vector<vec3> points_load_file_shp(const std::string& filename);

}
