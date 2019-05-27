#pragma once

#include "../vec/vec.hpp"

namespace vcl {

/** vec4 is an alias on the generic vec<4> */
using vec4 = vec<4>;

template <> struct vec<4> {

    float x;
    float y;
    float z;
    float w;

    vec<4>();
    vec<4>(float x,float y,float z,float w);

    const float& operator[](std::size_t index) const;
    float& operator[](std::size_t index);

};



}
