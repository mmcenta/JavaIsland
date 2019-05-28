
#include "00_introduction.hpp"

#include <random>

#ifdef INF443_INTRODUCTION

// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::
using namespace vcl;




/** This function is called before the beginning of the animation loop
    It is used to initialize all part-specific data */
void scene_exercise::setup_data(std::map<std::string,GLuint>& map, scene_structure& scene, gui_structure& )
{
    // Create mesh structure (stored on CPU)
    // *************************************** //
    mesh quadrangle;
    // Fill position buffer to model a unit quadrangle
    quadrangle.position = {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0}};
    // Set the connectivity (quadrangle made of two triangles)
    quadrangle.connectivity = {{0,1,2}, {0,2,3}};

    // Transfert mesh onto GPU (ready to be displayed)
    // *************************************** //

    // Convert mesh structure to mesh_gpu that stores VBO and VAO
    mesh_gpu quadrangle_gpu = mesh_gpu(quadrangle); // send data on GPU in the constructor of mesh_gpu

    // Convert mesh_gpu to mesh_drawable (higher level structure allowing to easily set uniform parameters)
    surface = mesh_drawable(quadrangle_gpu); // conversion performed using the constructor of mesh_drawable

    // Example of uniform parameter setting: color of the shape (used in the shader)
    surface.uniform_parameter.color = {1.0f, 0.3f, 0.6f};

    // Create a mesh approximating a sphere (unit radius by default)
    mesh sphere_cpu = mesh_primitive_sphere(); // mesh_primitive_sphere is a helper function (several primitive are available)
    // Send sphere_cpu data onto GPU
    sphere = mesh_drawable(sphere_cpu);
    // Set uniform parameter of the sphere
    sphere.uniform_parameter.color = {1,0,0};                  //red sphere
    sphere.uniform_parameter.translation = {-0.1f,0.5f,0.25f}; // translate sphere display
    sphere.uniform_parameter.scaling = 0.1f;                   // scale the sphere to new radius for its display

}




/** This function is called at each frame of the animation loop.
    It is used to compute time-varying argument and perform data data drawing */
void scene_exercise::frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& )
{
    // Drawing call: provide the shader and camera information
    surface.draw(shaders["wireframe"], scene.camera);
    sphere.draw(shaders["mesh"], scene.camera);
}




#endif

