#pragma once

#include "vcl/core/math/vec/vec3/vec3.hpp"
#include "vcl/core/scene/camera/camera.hpp"

namespace vcl
{

struct ray
{
    ray();
    ray(const vec3& p, const vec3& u);
    vec3 p; // position
    vec3 u; // direction
};

ray picking_ray(const camera_scene& camera, float x, float y);

}
