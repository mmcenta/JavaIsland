#include "vec3.hpp"

#include <iostream>
#include <cassert>

namespace vcl {

vec3::vec()
    :x(0),y(0),z(0)
{}

vec3::vec(float x_arg, float y_arg, float z_arg)
    :x(x_arg),y(y_arg),z(z_arg)
{}

const float& vec3::operator[](std::size_t index) const
{
    switch(index) {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        std::cerr<<"Error: Try to access vec3["<<index<<"]"<<std::endl;
        assert(false);
    }
	abort();
}
float& vec3::operator[](std::size_t index)
{
    switch(index) {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        std::cerr<<"Error: Try to access vec3["<<index<<"]"<<std::endl;
        assert(false);
    }
	abort();
}

vec3 cross(const vec3& a,const vec3& b)
{
    return    { a.y*b.z-a.z*b.y,
                a.z*b.x-a.x*b.z,
                a.x*b.y-a.y*b.x };
}

}
