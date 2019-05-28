#pragma once

#include "../vec/vec.hpp"

namespace vcl {

/** vec2 is an alias on a generic vec<2> */
using vec2 = vec<2>;

template <> struct vec<2> {

    float x;
    float y;

    vec<2>();
    vec<2>(float x,float y);

    const float& operator[](std::size_t index) const;
    float& operator[](std::size_t index);
};



}
