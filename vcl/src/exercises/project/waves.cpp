
#include "raytracing.hpp"
#include "waves.hpp"

#include <random>

// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::
using namespace vcl;

// Generator for uniform random number on the interaval [-1.0, +1.0]
std::default_random_engine gen;
std::uniform_real_distribution<float> unif(-1.0,1.0);

mesh create_cylinder(float radius, float height, float offset);

// Constants
const float PI = 3.14159265358979323846f;

// Global varibles for simulation
float t = 0.0f;
float dt = 0.02f;

float df = 0.2f;
float f0 = 1.8f;
float f1 = 2.8f;

float dphi = 0.1;
float wind_speed = 50;

/** This function is called before the beginning of the animation loop
    It is used to initialize all part-specific data */
void scene_exercise::setup_data(std::map<std::string,GLuint>& , scene_structure& scene, gui_structure& ) {
    // setup initial position of the camera
    scene.camera.camera_type = camera_control_spherical_coordinates;
    scene.camera.scale = 50.0f;
    scene.camera.apply_rotation(0,0,0,1.2f);

    // Set 
    sky = create_cylinder(140,100,40);
    texture_sun_billboard= texture_gpu(image_load_png("data/sun.png"));
    init_cloud_textures();

    billboard_surface = create_billboard_surface(70.0);
    billboard_surface2 = create_billboard_surface(40.0);
    billboard_surface.uniform_parameter.shading = { 1, 0, 0 };
    billboard_surface2.uniform_parameter.shading = { 1, 0, 0 };
    update_cloud_position(130, -10, 7, 0, 800);
    update_cloud_position(130, 7, 7, 0, 500);
    update_cloud_position(130, 13, 10, 0, 50);
    update_cloud_position(130, 16, 8, 0, 27);
    update_cloud_position(130, 80, 20, 0, 400);
    sun_position= vec3(120, 0, 3);

    // Define the base plane of the ocean
    ocean_plane.point = { 0.0, 0.0, 0.0 };
    ocean_plane.normal = { 0.0, 0.0, 1.0 };

    // Create a grid of rays that are projected from the camera 
    grid = new ray_grid(90, 160);

    // Initialize wave trains
    init_waves();

    // Create mesh to display the segments
    segment_drawer.init();
    segment_drawer.uniform_parameter.color = {0, 0, 1};

    // Create the sphere to display the projected rays
    const float r = 0.01f;
    sphere = mesh_primitive_sphere(r);
    sphere.uniform_parameter.color = {0.6f, 0.6f, 1.0f};

    // Create the mesh representing the ocean
    ocean = mesh_primitive_grid(90, 160, 5.0f, 5.0f, { -2.5f, -2.5f, 0.0f }); 
    ocean.uniform_parameter.color = {0.1f, 0.1f, 1.0f};
    ocean.uniform_parameter.shading.specular = 200.0f;
    ocean.uniform_parameter.shading.ambiant = 0.1f;
    ocean.uniform_parameter.shading.diffuse = 0.5f;
}

void scene_exercise::init_cloud_textures(){
    for(int i=0; i < 11; i++){
        texture_cloud_billboard[i]= texture_gpu(image_load_png("data/"+to_string(i+1)+".png"));
    }
}

void scene_exercise::init_waves() {
    // Physical constants (as per http://www.smart-coding.net/irwave/)
    const float g = 9.81f;
    const float A = 0.0081 * pow(g, 2);
    const float B = 0.74 * pow(g/wind_speed, 4);  
    
    // Start generating waves
    float f = f0;

    num_waves = 0;
    while (f < f1) {   
        for (int i =0; i < 4; i++){
            wave w;
            const float u = 2*PI*unif(gen);

            const float S = A * pow(f,-5) * exp(-B/pow(f,4)) * (8/(3*PI)) * pow(cos(u), 4);
            const float speed = pow(f, 2)/g;
            const float amplitude = sqrt(2*S*df); 

            w.amplitude = amplitude;
            w.frequency = f;
            w.direction = vec2(speed*std::cos(u), speed*std::sin(u));

            waves.emplace_back(w);
            ++num_waves;
        }
       f += df;
    }
}

/*  This function implements the Gerstner model for simulating waves with a dynamically 
 *  adapted surface mesh.
 *  
 *  view_distance: is the maximum distance that will render objects.
 *  camera_position: is where the camera currently is.
 */
void scene_exercise::update_mesh_ocean(float view_distance, const vec3& camera_position) {
    std::vector<vec3> positions;
    std::vector<vec3> normals;

    for (int i = 0; i < grid->height; ++i) {
        for (int j = 0; j < grid->width; ++j) {
            const picking_info intersect = ray_intersect_plane(grid->rays[i][j],
                                            ocean_plane.normal, ocean_plane.point);

            if (intersect.picking_valid) {
                const vec2 X0 = { intersect.intersection[0], intersect.intersection[1] };
                const float z0 = ocean_plane.point[2];

                // Compute displacements and normals on the Gerstner model
                vec2 X = X0;
                float z = z0;
                vec3 normal = { 0.0f, 0.0f, 1.0f };
                for (int k = 0; k < num_waves; ++k) {
                    // Displacement on the plane of the ocean
                    const vec2 var_plane = waves[k].amplitude * (waves[k].direction / norm(waves[k].direction))
                                           * std::sin(waves[k].frequency*t - dot(waves[k].direction, X0));
                    // Displacement in height
                    const float var_height = waves[k].amplitude * std::cos(waves[k].frequency*t - dot(waves[k].direction, X0));

                    // Update particle position
                    X += var_plane;
                    z += var_height;

                    // Update normal
                    normal[0] -=  waves[k].amplitude * std::sin(waves[k].frequency*t - dot(waves[k].direction, X0))*waves[k].direction[0];
                    normal[1] -=  waves[k].amplitude * std::sin(waves[k].frequency*t - dot(waves[k].direction, X0))*waves[k].direction[1];
                }
                positions.emplace_back(X[0], X[1], z);
                normals.push_back(normal/norm(normal));
            }
            else {
                // Project the ray direction onto the ocean plane
                const vec3 ray_u_plane = grid->rays[i][j].u - dot(grid->rays[i][j].u, ocean_plane.normal)*ocean_plane.normal;

                // Place the point on this direction beyond the view distance
                positions.emplace_back(camera_position[0] + 1.1*view_distance*ray_u_plane[0],
                                       camera_position[0] + 1.1*view_distance*ray_u_plane[1],
                                                                        ocean_plane.point[2]);

                normals.push_back(ocean_plane.normal);
            }
        }
    }

    t += dt;

    // Update the mesh on the GPU data
    ocean.data_gpu.update_position(positions);
    ocean.data_gpu.update_normal(normals);
}

/** This function is called at each frame of the animation loop.
    It is used to compute time-varying argument and perform data data drawing */
void scene_exercise::frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& ) {
    set_gui();
    glEnable( GL_POLYGON_OFFSET_FILL ); // avoids z-fighting when displaying wireframe
    
    // Update wave simulation
    update_rays(grid, scene.camera);
    update_mesh_ocean(scene.camera.perspective.z_far, scene.camera.camera_position());

    // Display skybox
    sky.draw(shaders["sky"], scene.camera);
    display_clouds(shaders, scene);
    display_sun(shaders, scene);
    
    // Display ocean
    if(gui_scene.wireframe)
        ocean.draw(shaders["wireframe"], scene.camera);
    else
        ocean.draw(shaders["water"], scene.camera);
}

vcl::mesh scene_exercise::create_billboard_surface(float size) {
    mesh billboard;
    billboard.position = { {-0.1f*size, 0, 0},
                           { 0.1f*size, 0, 0},
                           { 0.1f*size, 0.2f*size, 0},
                           {-0.1f*size, 0.2f*size, 0}};
     
    billboard.texture_uv = { { 0, 1}, { 1, 1}, { 1, 0}, { 0, 0}};
    billboard.connectivity = { { 0, 1, 2}, { 0, 2, 3}};

    return billboard;
}

void scene_exercise::update_cloud_position(float radius, float height, float ecart, float ecart2, int N_clouds) {
    for (int k = 0; k < N_clouds; ++k) {
        const float h_var = height + ecart * unif(gen);
        float ang_var = 2*PI * unif(gen);
        float dist_var = unif(gen);
        const float x = (radius + dist_var*ecart2)*sin(ang_var);
        const float y = (radius + dist_var*ecart2)*cos(ang_var);
        
        clouds_position.push_back(vec3(x , y, height + h_var));
    }
}

void scene_exercise::display_sun(std::map<std::string,GLuint>& shaders, scene_structure& scene) {
    glEnable(GL_BLEND);
    glDepthMask(false);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, texture_sun_billboard);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    billboard_surface2.uniform_parameter.rotation = scene.camera.orientation;
    billboard_surface2.uniform_parameter.scaling = 5.0f;
    billboard_surface2.uniform_parameter.translation = sun_position;

    glPolygonOffset( 1.0, 1.0 );
    billboard_surface2.draw(shaders["mesh"], scene.camera);
    glBindTexture(GL_TEXTURE_2D, scene.texture_white);
    glDepthMask(true);
}

void scene_exercise::display_clouds(std::map<std::string,GLuint>& shaders, scene_structure& scene) {
    if(!gui_scene.clouds)
        return;

    const size_t N_clouds = clouds_position.size();
    if(gui_scene.cloudstexture){
        for(int i =0; i < 11; i++){
            glEnable(GL_BLEND);
            glDepthMask(false);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBindTexture(GL_TEXTURE_2D, texture_cloud_billboard[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            billboard_surface.uniform_parameter.rotation = scene.camera.orientation;
            billboard_surface.uniform_parameter.scaling = 1.5f;

            for(size_t k=0; k<N_clouds/11; ++k)
            {
                const vec3& p = clouds_position[i*N_clouds/12+k];
                billboard_surface.uniform_parameter.translation = p;

                glPolygonOffset( 1.0, 1.0 );
                billboard_surface.draw(shaders["mesh"], scene.camera);
            }

            
            glBindTexture(GL_TEXTURE_2D, scene.texture_white);
            glDepthMask(true);
        }
    }
}

mesh create_cylinder(float radius, float height, float offset) {
    mesh m;

    // Number of samples
    const size_t N = 20;

    // Geometry
    for(size_t k=0; k<N; ++k)
    {
        const float u = k/float(N);
        const vec3 p = {radius*std::cos(2*3.14f*u), radius*std::sin(2*3.14f*u), 0.0f};
        m.position.push_back( p+vec3(0,0,-offset) );
        m.color.push_back(vec4(4,4,4,1));
        m.position.push_back( p+vec3(0,0,height) );
        m.color.push_back(vec4(0,2,4,1));
    }

    // Connectivity
    for(size_t k=0; k<N; ++k)
    {
        const unsigned int u00 = 2*k;
        const unsigned int u01 = (2*k+1)%(2*N);
        const unsigned int u10 = (2*(k+1))%(2*N);
        const unsigned int u11 = (2*(k+1)+1) % (2*N);

        const index3 t1 = {u00, u10, u11};
        const index3 t2 = {u00, u11, u01};
        m.connectivity.push_back(t1);
        m.connectivity.push_back(t2);
    }

    return m;
}

void scene_exercise::set_gui() {
    ImGui::Checkbox("Wireframe", &gui_scene.wireframe);
    ImGui::Checkbox("Clouds", &gui_scene.clouds);
    ImGui::Checkbox("Cloud Texture", &gui_scene.cloudstexture);

    float scaling_min = 0.01f;
    float scaling_max = 0.8f;
    if( ImGui::SliderScalar("dt", ImGuiDataType_Float, &gui_scene.scaling, &scaling_min, &scaling_max) )
        dt = gui_scene.scaling;
}