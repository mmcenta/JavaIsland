#pragma once

#include <vector>
#include "../../vcl/core/math/math.hpp"


struct particle {
    vcl::vec3 position;
    vcl::vec3 normal;
};

struct particle_grid {
    int rows, columns;
    std::vector<std::vector<particle>> particles;

    particle_grid(int rows, int columns) :
        rows(rows), columns(columns),
        particles(rows, std::vector<particle>(columns)) { };
};
