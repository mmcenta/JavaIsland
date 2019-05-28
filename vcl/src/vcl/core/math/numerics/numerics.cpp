#include "numerics.hpp"

#include <cmath>

namespace vcl
{

bool is_equal(float a, float b)
{
    return std::abs(a-b)<1e-6f;
}

}
