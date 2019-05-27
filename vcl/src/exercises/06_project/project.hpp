#pragma once

#include "../../exercises/base_exercise/base_exercise.hpp"

#ifdef INF443_PROJECT

struct oray {
    vcl::vec3 origin;
    vcl::vec3 direction;
};

struct plane {
    vcl::vec3 point;
    vcl::vec3 normal;
};

struct grid {
    int width;
    int height; 

    std::vector<std::vector<vcl::vec3>> points;
};


struct scene_exercise : base_scene_exercise {
    void setup_data(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);
    void frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);

    plane ocean_plane;

    std::vector<std::vector<oray>> rays;
    grid ogrid;

    vcl::mesh_drawable ocean;
    vcl::mesh_drawable sphere;
    vcl::segment_drawable_immediate_mode segment_drawer;
};

#endif