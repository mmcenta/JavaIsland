#include "project.hpp"


#ifdef INF443_PROJECT

using namespace vcl;


void update_rays(std::vector<std::vector<oray>>& rays, grid& g, camera_scene& camera) {
    const mat4 R = camera.camera_matrix();
    const vec3 p0 = camera.camera_position();
    const float d = 1/std::tan(camera.perspective.angle_of_view/2);
    const float aspect_ratio = camera.perspective.image_aspect;

    const float w_step = 2.0/g.width;
    const float h_step = 2.0/g.height;

    for (int i = 0; i < g.height; ++i) {
        for (int j = 0; j < g.width; ++j) {
            vec4 p_grid = {(-1.0f + (j + 0.5f)*w_step)*aspect_ratio,
                                           1.0f - (i + 0.5f)*h_step,
                                                                 -d,
                                                               1.0f };
            p_grid = R*p_grid; 
            
            const vec3 p1 = { p_grid[0]/p_grid[3],
                              p_grid[1]/p_grid[3],
                              p_grid[2]/p_grid[3] };
            const vec3 dir = normalize(p1-p0);

            rays[i][j].origin = p0;
            rays[i][j].direction = dir;
        }
    }
}

vec3 intersect_ray_plane(const oray& l, const plane& p) {
    const float dot_prod = dot(l.direction, p.normal);

    if (dot_prod != 0) {
        const float d = dot(p.point - l.origin, p.normal)/dot_prod;   

        return (l.origin + d*l.direction);
    }

    return { 0.0, 0.0, 0.0};
}

void update_grid(grid& g, std::vector<std::vector<oray>> rays, plane& p) {
    for (int i = 0; i < g.height; ++i)
        for (int j = 0; j < g.width; ++j)
            g.points[i][j] = intersect_ray_plane(rays[i][j], p);
}

void scene_exercise::setup_data(std::map<std::string,GLuint>& , scene_structure& , gui_structure& ) {
    // Create plane representing ocean
    ocean_plane.point = { 0.0, 0.0, 0.0 };
    ocean_plane.normal = { 0.0, 0.0, 1.0 };

    // Create grid of points
    ogrid.width = 160;
    ogrid.height = 90;
    
    ogrid.points.resize(ogrid.height);
    for (int i = 0; i < ogrid.height; ++i)
        ogrid.points[i].resize(ogrid.width);

    // Create lines
    rays.resize(ogrid.height);
    for (int i = 0; i < ogrid.height; ++i)
        rays[i].resize(ogrid.width);

    // Create mesh to display the segments
    segment_drawer.init();
    segment_drawer.uniform_parameter.color = {0,0,1};

    // Create the sphere to display the points
    const float r = 0.01f;
    sphere = mesh_primitive_sphere(r);
    sphere.uniform_parameter.color = {0.6f, 0.6f, 1.0f};

    // Create the grid representing the ocean
    ocean = mesh_primitive_grid(20, 20, 5.0f, 5.0f, { -2.5f, -2.5f, 0.0f }); 
    ocean.uniform_parameter.color = {0.1f, 0.1f, 1.0f};
}


void scene_exercise::frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& ) {
    update_rays(rays, ogrid, scene.camera);
    update_grid(ogrid, rays, ocean_plane);

    /*
    // Display the lines
    const float k = 1.0;

    for (int i = 0; i < pgrid.height - 1; ++i) {
        for (int j = 0; j < pgrid.width - 1; ++j) {
            segment_drawer.uniform_parameter.p1 = lines[i][j].point;
            segment_drawer.uniform_parameter.p2 = lines[i][j].point + k*lines[i][j].direction;
            segment_drawer.draw(shaders["segment_im"],scene.camera);
        }
    }
    */
    // Display ocean
    ocean.draw(shaders["wireframe"], scene.camera);

    // Display spheres
    for (int i = 0; i < ogrid.height; ++i) {
        for (int j = 0; j < ogrid.width; ++j) {
            sphere.uniform_parameter.translation = ogrid.points[i][j];       
            sphere.draw(shaders["mesh"], scene.camera);
        }
    }
}

#endif
