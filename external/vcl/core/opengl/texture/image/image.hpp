#pragma once

#include <vector>

namespace vcl
{

struct image
{
    unsigned int width;
    unsigned int height;
    std::vector<unsigned char> data;
};

}
