#pragma once

#include "vcl/core/opengl/segments_gpu/segments_gpu.hpp"
#include "vcl/core/scene/camera/camera.hpp"
#include "../drawable_uniform/drawable_uniform.hpp"

namespace vcl
{

/** Allow to draw segment in immediate mode
    Extremities of each segment are set as uniform parameters */
struct segment_drawable_immediate_mode
{
public:
    segment_drawable_immediate_mode();
    void init();

    struct
    {
        vec3 p1;
        vec3 p2;
        vec3 color;
    } uniform_parameter;

    segments_gpu data_gpu;

    void draw(GLuint shader, const camera_scene& camera);

private:
    bool initialized;
};

}
