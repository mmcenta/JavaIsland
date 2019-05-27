#include "vec2.hpp"

#include <iostream>
#include <cassert>

namespace vcl {

vec2::vec()
    :x(0),y(0)
{}

vec2::vec(float x_arg, float y_arg)
    :x(x_arg),y(y_arg)
{}

const float& vec2::operator[](std::size_t index) const
{
    switch(index) {
    case 0:
        return x;
    case 1:
        return y;
    default:
        std::cerr<<"Error: Try to access vec2["<<index<<"]"<<std::endl;
        assert(false);
    }
	abort();
}
float& vec2::operator[](std::size_t index)
{
    switch(index) {
    case 0:
        return x;
    case 1:
        return y;
    default:
        std::cerr<<"Error: Try to access vec2["<<index<<"]"<<std::endl;
        assert(false);
    }
	abort();
}



}
