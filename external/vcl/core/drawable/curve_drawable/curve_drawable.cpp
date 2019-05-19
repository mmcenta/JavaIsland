#include "curve_drawable.hpp"

#include "vcl/core/opengl/opengl.hpp"

namespace vcl
{

curve_drawable::curve_drawable()
    :uniform_parameter(),data_gpu()
{}

curve_drawable::curve_drawable(const curve_gpu& data)
    :uniform_parameter(),data_gpu(data)
{}
curve_drawable::curve_drawable(const std::vector<vec3>& data)
    :uniform_parameter(),data_gpu(curve_gpu(data))
{}

void curve_drawable::draw(GLuint shader, const camera_scene& camera)
{
                                                                    opengl_debug();
    GLint current_shader = 0;                                       opengl_debug();
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_shader);             opengl_debug();

    if(shader!=GLuint(current_shader))
        glUseProgram(shader);                                       opengl_debug();

    uniform(shader, "rotation", uniform_parameter.rotation);        opengl_debug();
    uniform(shader, "translation", uniform_parameter.translation);  opengl_debug();
    uniform(shader, "color", uniform_parameter.color);              opengl_debug();
    uniform(shader, "scaling", uniform_parameter.scaling);          opengl_debug();

    uniform(shader,"perspective",camera.perspective.matrix());      opengl_debug();
    uniform(shader,"view",camera.view_matrix());                    opengl_debug();

    vcl::draw(data_gpu);                                            opengl_debug();
}

}
