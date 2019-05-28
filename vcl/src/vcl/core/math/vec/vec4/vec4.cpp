#include "vec4.hpp"

#include <iostream>
#include <cassert>

namespace vcl {

vec4::vec()
    :x(0),y(0),z(0),w(0)
{}

vec4::vec(float x_arg, float y_arg, float z_arg,float w_arg)
    :x(x_arg),y(y_arg),z(z_arg),w(w_arg)
{}

const float& vec4::operator[](std::size_t index) const
{
    switch(index) {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        std::cerr<<"Error: Try to access vec4["<<index<<"]"<<std::endl;
        assert(false);
    }
	abort();
}
float& vec4::operator[](std::size_t index)
{
    switch(index) {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        std::cerr<<"Error: Try to access vec4["<<index<<"]"<<std::endl;
        assert(false);
    }
	abort();
}



}
