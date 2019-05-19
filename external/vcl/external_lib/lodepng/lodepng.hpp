#pragma once

#include "../headers/lodepng.hpp"

#include "vcl/core/opengl/texture/image/image.hpp"

#include <string>

namespace vcl
{

image image_load_png(const std::string& filename);
void image_save_png(const std::string& filename, const image& im);

}
