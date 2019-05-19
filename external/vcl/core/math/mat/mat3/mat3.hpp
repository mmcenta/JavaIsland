#pragma once

#include "../mat/mat.hpp"
#include "../../vec/vec3/vec3.hpp"

namespace vcl {
using mat3 = mat<3,3>;

template <>
struct mat<3,3> {

    float xx,xy,xz;
    float yx,yy,yz;
    float zx,zy,zz;

    mat<3,3>();
    mat<3,3>(float xx,float xy,float xz,
             float yx,float yy,float yz,
             float zx,float zy,float zz);


    static mat3 zero();
    static mat3 identity();

    vec3 row(std::size_t offset) const;
    vec3 col(std::size_t offset) const;
    mat3& set_row(std::size_t offset, const vec3& v);
    mat3& set_col(std::size_t offset, const vec3& v);


    const float& operator[](std::size_t offset) const;
    float& operator[](std::size_t offset);

    const float& operator()(std::size_t index1, std::size_t index2) const;
    float& operator()(std::size_t index1, std::size_t index2);
};

float det(const mat3& m);
mat3 inverse(const mat3& m);





}
