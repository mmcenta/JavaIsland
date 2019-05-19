#pragma once

#include "vcl/core/opengl/segments_gpu/segments_gpu.hpp"
#include "vcl/core/scene/camera/camera.hpp"
#include "../drawable_uniform/drawable_uniform.hpp"

namespace vcl
{

struct segments_drawable
{
public:
    segments_drawable();
    segments_drawable(const segments_gpu& data);
    segments_drawable(const std::vector<vec3>& data);

    drawable_uniform uniform_parameter;
    segments_gpu data_gpu;

    void draw(GLuint shader, const camera_scene& camera);
};

}
