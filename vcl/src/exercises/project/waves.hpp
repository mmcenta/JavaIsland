#pragma once

#include "pixel_ray.hpp"
#include "../../exercises/base_exercise/base_exercise.hpp"


struct wave {
    float frequency;
    float amplitude;
    vcl::vec2 direction;
};

// Stores some parameters that can be set from the GUI
struct gui_scene_structure
{
    bool wireframe = true;
    bool clouds  = true;
    bool cloudstexture  = true;
    bool wireframeclouds  = false;
    bool skybox = false;
    float scaling = 0.02f;
    float threshold = 0.2f;
};

struct scene_exercise : base_scene_exercise
{

    /** A part must define two functions that are called from the main function:
     * setup_data: called once to setup data before starting the animation loop
     * frame_draw: called at every displayed frame within the animation loop
     *
     * These two functions receive the following parameters
     * - shaders: A set of shaders.
     * - scene: Contains general common object to define the 3D scene. Contains in particular the camera.
     * - data: The part-specific data structure defined previously
     * - gui: The GUI structure allowing to create/display buttons to interact with the scene.
    */
    gui_scene_structure gui_scene;

    void setup_data(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);
    void frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);

    void set_gui();

    // visual representation of a surface
    vcl::mesh_drawable sky;

    // Waves
    int num_waves;
    std::vector<wave> waves;
    void init_waves();

    vcl::mesh create_billboard_surface(float size);
    vcl::mesh_drawable billboard_surface;
    vcl::mesh_drawable billboard_surface2;
    GLuint texture_cloud_billboard[12];
    std::vector<vcl::vec3> clouds_position;
    void init_textures();
    void update_cloud_position(float radius, float height, float ecart,  float ecart2, int N_clouds);
    void display_clouds(std::map<std::string,GLuint>& shaders, scene_structure& scene);

    GLuint texture_sun_billboard;
    vcl::vec3 sun_position;
    void display_sun(std::map<std::string,GLuint>& shaders, scene_structure& scene);

    // Ocean-related data and methods
    plane ocean_plane;
    vcl::mesh_drawable ocean;

    void update_mesh_ocean();

    // Local grid of projected rays
    ray_grid *grid;

    // for debug
    vcl::mesh_drawable sphere;
    vcl::segment_drawable_immediate_mode segment_drawer;
};


