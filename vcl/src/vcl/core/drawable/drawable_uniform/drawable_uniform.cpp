#include "drawable_uniform.hpp"

namespace vcl
{

drawable_uniform::drawable_uniform()
    :rotation({1,0,0, 0,1,0, 0,0,1}),translation({0,0,0}),scaling(1.0f),scaling_axis(1.0f,1.0f,1.0f),color({1,1,1}),shading({0.2f,0.8f,0.5f})
{}


}
