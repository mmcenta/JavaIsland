#pragma once

#include "vcl/core/opengl/curve_gpu/curve_gpu.hpp"
#include "vcl/core/scene/camera/camera.hpp"
#include "../drawable_uniform/drawable_uniform.hpp"

namespace vcl
{

struct curve_drawable
{
public:
    curve_drawable();
    curve_drawable(const curve_gpu& data);
    curve_drawable(const std::vector<vec3>& data);

    drawable_uniform uniform_parameter;
    curve_gpu data_gpu;

    void draw(GLuint shader, const camera_scene& camera);
};

}
