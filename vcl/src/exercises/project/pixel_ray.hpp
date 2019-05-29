#pragma once

#include <vector>
#include "../../vcl/vcl.hpp"


struct pixel_ray {
    vcl::vec3 origin;
    vcl::vec3 direction;
};

struct plane {
    vcl::vec3 point;
    vcl::vec3 normal;
};

struct ray_grid {
    int height, width;
    std::vector<std::vector<vcl::ray>> rays;

    ray_grid(int height, int width) :
        height(height), width(width),
        rays(height, std::vector<vcl::ray>(width)) { };
};


void update_rays(ray_grid *rgrid, const vcl::camera_scene& camera);
vcl::vec3 intersect_ray_plane(const pixel_ray& r, const plane& p);