#pragma once

#include <vector>
#include "../../vcl/vcl.hpp"


// Data structure representing a plane by a member point and the normal vector
struct plane {
    vcl::vec3 point;
    vcl::vec3 normal;
};

// Data structure representing a grid of projected rays
struct ray_grid {
    int height, width; // number of ray rows and columns (seen as a screen)
    std::vector<std::vector<vcl::ray>> rays;

    ray_grid(int height, int width) :
        height(height), width(width),
        rays(height, std::vector<vcl::ray>(width)) { };
};

/* 
 *  This function updates a ray grid to contain rays projected from
 * the camera through a grid of width and height given by the grid
 * of rays.
 */
void update_rays(ray_grid *rgrid, const vcl::camera_scene& camera);