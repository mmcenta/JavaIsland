#include "../../vcl/vcl.hpp"
#include "raytracing.hpp"

using namespace vcl;


void update_rays(ray_grid *g, const camera_scene& camera) {
    const mat4 R = camera.camera_matrix();
    const vec3 p0 = camera.camera_position();
    const float d = 1/std::tan(1.2f*camera.perspective.angle_of_view/2);
    const float aspect_ratio = camera.perspective.image_aspect;

    const float w_step = 2.0/g->width;
    const float h_step = 2.0/g->height;

    for (int i = 0; i < g->height; ++i) {
        for (int j = 0; j < g->width; ++j) {
            vec4 p_grid = {(-1.0f + (j + 0.5f)*w_step)*aspect_ratio,
                                           1.0f - (i + 0.5f)*h_step,
                                                                 -d,
                                                               1.0f };
            p_grid = R*p_grid; 
            
            const vec3 p1 = { p_grid[0]/p_grid[3],
                              p_grid[1]/p_grid[3],
                              p_grid[2]/p_grid[3] };
            const vec3 dir = normalize(p1-p0);

            g->rays[i][j].p = p0;
            g->rays[i][j].u = dir;
        }
    }
}
