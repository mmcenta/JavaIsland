
// OpenGL Loader must be included first
#include "vcl/external_lib/headers/glad.hpp"
#include "vcl/external_lib/headers/glfw.hpp"

#include <string>

namespace vcl
{


/** Initialize GLFW.
 * Function should be called before any use of GLFW
 * Exit program if fails */
void glfw_init();

GLFWwindow* glfw_create_window(int width, int height, const std::string& title, int opengl_version_major, int opengl_version_minor, GLFWmonitor* monitor=nullptr, GLFWwindow* share=nullptr);

void glad_init();

}
