#pragma once

#include "../vec/vec.hpp"

namespace vcl {

/** vec3 is an alias on a generic vec<3> */
using vec3 = vec<3>;

/** vec3 is a specialized-template class from a generic vec<N> */
template <> struct vec<3> {

    /** Three x,y,z coordinates of the vector */
    float x;
    float y;
    float z;

    vec<3>();
    vec<3>(float x,float y,float z);

    /** Get operator at given index */
    const float& operator[](std::size_t index) const;
    /** Set operator at given index */
    float& operator[](std::size_t index);
};

/** Cross product between two vec3 */
vec3 cross(const vec3& a,const vec3& b);


}
