#pragma once

#include "vcl/core/math/vec/vec3/vec3.hpp"
#include "vcl/core/math/mat/mat3/mat3.hpp"

namespace vcl
{

struct drawable_uniform
{
    drawable_uniform();

    mat3 rotation;
    vec3 translation;
    float scaling;
    vec3 scaling_axis;
    vec3 color;


    struct
    {
        float ambiant;
        float diffuse;
        float specular;
    } shading ;
};


}
