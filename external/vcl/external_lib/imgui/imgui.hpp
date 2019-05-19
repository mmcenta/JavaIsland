#pragma once

#include "../headers/imgui.hpp"
#include "../headers/glfw.hpp"

namespace vcl
{

void imgui_init(GLFWwindow* window);

void imgui_create_frame();
void imgui_render_frame(GLFWwindow* window);
void imgui_cleanup();
}
