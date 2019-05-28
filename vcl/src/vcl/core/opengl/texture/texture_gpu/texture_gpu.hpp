#pragma once

#include "vcl/external_lib/headers/glad.hpp"
#include "../image/image.hpp"

namespace vcl
{

GLuint texture_gpu(const image& im);
GLuint texture_gpu_white();

}
