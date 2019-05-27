
#include "example_billboard.hpp"


#ifdef INF443_EXAMPLE_BILLBOARD

// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::
using namespace vcl;


/** This function is called before the beginning of the animation loop
    It is used to initialize all part-specific data */
void scene_exercise::setup_data(std::map<std::string,GLuint>& , scene_structure& , gui_structure& )
{
    // Create a quad with (u,v)-texture coordinates
    mesh surface_cpu;
    surface_cpu.position     = {{-0.2f,0,0}, { 0.2f,0,0}, { 0.2f, 0.4f,0}, {-0.2f, 0.4f,0}};
    surface_cpu.texture_uv   = {{0,1}, {1,1}, {1,0}, {0,0}};
    surface_cpu.connectivity = {{0,1,2}, {0,2,3}};

    surface = surface_cpu;
    surface.uniform_parameter.shading = {1,0,0}; // set pure ambiant component (no diffuse, no specular) - allow to only see the color of the texture


    // Load a texture (with transparent background)
    texture_id = texture_gpu( image_load_png("data/billboard_grass.png") );


    // Create a reference ground as a horizontal quadrangle
    ground = mesh_primitive_quad({-1,0,-1},{-1,0,1},{1,0,1},{1,0,-1});
}



/** This function is called at each frame of the animation loop.
    It is used to compute time-varying argument and perform data data drawing */
void scene_exercise::frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& )
{
    set_gui();

    // Display reference ground
    ground.draw(shaders["mesh"], scene.camera);


    // Enable use of alpha component as color blending for transparent elements
    //  new color = previous color + (1-alpha) current color
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Disable depth buffer writing
    //  - Transparent elements cannot use depth buffer
    //  - They are supposed to be display from furest to nearest elements
    glDepthMask(false);


    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // avoids sampling artifacts
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // avoids sampling artifacts

    // Display a billboard always facing the camera direction
    // ********************************************************** //
    surface.uniform_parameter.rotation = scene.camera.orientation;
    surface.uniform_parameter.translation = {0.25f,0,-0.5f};
    surface.draw(shaders["mesh"], scene.camera);
    if(gui_scene.wireframe)
        surface.draw(shaders["wireframe"], scene.camera);

    // Display two orthogonal billboards with static orientation
    // ********************************************************** //
    const mat3 Identity = mat3::identity();
    const mat3 R = rotation_from_axis_angle_mat3({0,1,0}, 3.14f/2.0f); // orthogonal rotation

    surface.uniform_parameter.translation = {0,0,0};

    surface.uniform_parameter.rotation = Identity;
    surface.draw(shaders["mesh"], scene.camera);
    if(gui_scene.wireframe)
        surface.draw(shaders["wireframe"], scene.camera);

    surface.uniform_parameter.rotation = R;
    surface.draw(shaders["mesh"], scene.camera);
    if(gui_scene.wireframe)
        surface.draw(shaders["wireframe"], scene.camera);


    glBindTexture(GL_TEXTURE_2D, scene.texture_white);
    glDepthMask(true);

}




void scene_exercise::set_gui()
{
    ImGui::Checkbox("Wireframe", &gui_scene.wireframe);
}



#endif

