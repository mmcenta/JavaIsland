#pragma once

#include "../../exercises/base_exercise/base_exercise.hpp"

#ifdef INF443_02_TEXTURE

// Stores some parameters that can be set from the GUI
struct gui_scene_structure
{
    bool wireframe = false;
    bool texture   = true;

    bool terrain   = true;
    bool wireframe_terrain = false;
    bool texture_terrain   = true;

    bool tree     = true;
    bool mushroom = true;
    bool flower   = true;
    bool grass    = true;

    bool skybox = false;
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

    void setup_data(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);
    void frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);

    void set_gui();

    // visual representation of a surface
    vcl::mesh_drawable terrain;
    GLuint texture_terrain;
    void display_terrain(std::map<std::string,GLuint>& shaders, scene_structure& scene);

    vcl::mesh_drawable tree_trunc;
    vcl::mesh_drawable tree_foliage;
    std::vector<vcl::vec3> tree_position;
    void update_tree_position();
    void display_tree(std::map<std::string,GLuint>& shaders, scene_structure& scene);


    vcl::mesh_drawable mushroom_trunc;
    vcl::mesh_drawable mushroom_top;
    std::vector<vcl::vec3> mushroom_position;
    void update_mushroom_position();
    void display_mushroom(std::map<std::string,GLuint>& shaders, scene_structure& scene);


    vcl::mesh_drawable billboard_surface;
    vcl::mesh create_billboard_surface();
    GLuint texture_flower_billboard;
    GLuint texture_grass_billboard;
    std::vector<vcl::vec3> flower_position;
    std::vector<vcl::vec3> grass_position;
    void update_flower_position();
    void update_grass_position();
    void display_flower(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void display_grass(std::map<std::string,GLuint>& shaders, scene_structure& scene);


    vcl::mesh_drawable skybox;
    GLuint texture_skybox;
    void display_skybox(std::map<std::string,GLuint>& shaders, scene_structure& scene);

    gui_scene_structure gui_scene;
};

#endif


