#include "mesh_drawable.hpp"

#include "vcl/core/opengl/opengl.hpp"

namespace vcl
{

mesh_drawable::mesh_drawable()
    :uniform_parameter(),data_gpu()
{}

mesh_drawable::mesh_drawable(const mesh_gpu& data)
    :uniform_parameter(),data_gpu(data)
{}
mesh_drawable::mesh_drawable(const mesh& data)
    :uniform_parameter(),data_gpu(mesh_gpu(data))
{}

void mesh_drawable::draw(GLuint shader, const camera_scene& camera)
{
    opengl_debug();
    GLint current_shader = 0; opengl_debug();
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_shader); opengl_debug();

    if(shader!=GLuint(current_shader))
        glUseProgram(shader); opengl_debug();



    uniform(shader, "rotation", uniform_parameter.rotation); opengl_debug();
    uniform(shader, "translation", uniform_parameter.translation); opengl_debug();
    uniform(shader, "color", uniform_parameter.color); opengl_debug();
    uniform(shader, "scaling", uniform_parameter.scaling); opengl_debug();
    uniform(shader, "scaling_axis", uniform_parameter.scaling_axis); opengl_debug();

    uniform(shader,"perspective",camera.perspective.matrix()); opengl_debug();
    uniform(shader,"view",camera.view_matrix()); opengl_debug();
    uniform(shader,"camera_position",camera.camera_position()); opengl_debug();




    uniform(shader, "ambiant", uniform_parameter.shading.ambiant); opengl_debug();
    uniform(shader, "diffuse", uniform_parameter.shading.diffuse); opengl_debug();
    uniform(shader, "specular", uniform_parameter.shading.specular); opengl_debug();

    vcl::draw(data_gpu);opengl_debug();
}

}
