
// Include VCL library
#include "vcl/vcl.hpp"

// Include common part for exercises
#include "helper_scene.hpp"

// Include exercises
#include "exercises/exercises.hpp"




// ************************************** //
// Global data declaration
// ************************************** //

// Storage for shaders indexed by their names
std::map<std::string,GLuint> shaders;

// General shared elements of the scene such as camera and its controler, visual elements, etc
scene_structure scene;

// The graphical interface. Contains Window object and GUI related variables
gui_structure gui;

// Part specific data - you will specify this object in the corresponding exercise part
scene_exercise exercise;


// ************************************** //
// GLFW event listeners
// ************************************** //

void window_size_callback(GLFWwindow* /*window*/, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);


// ************************************** //
// Start program
// ************************************** //

int main()
{

    // ************************************** //
    // Initialization and data setup
    // ************************************** //

    // Initialize external libraries and window
    initialize_interface(gui);

    // Set GLFW events listener
    glfwSetCursorPosCallback(gui.window, cursor_position_callback );
    glfwSetMouseButtonCallback(gui.window, mouse_click_callback);
    glfwSetWindowSizeCallback(gui.window, window_size_callback);

    load_shaders(shaders);
    setup_scene(scene, gui);


    opengl_debug();
    std::cout<<"*** Setup Data ***"<<std::endl;
    exercise.setup_data(shaders, scene, gui);
    std::cout<<"\t [OK] Data setup"<<std::endl;
    opengl_debug();


    // ************************************** //
    // Animation loop
    // ************************************** //


    std::cout<<"*** Start GLFW loop ***"<<std::endl;
    vcl::glfw_fps_counter fps_counter;
    while( !glfwWindowShouldClose(gui.window) )
    {
        opengl_debug();

        // Clear all color and zbuffer information before drawing on the screen
        clear_screen();opengl_debug();
        // Set a white image texture by default
        glBindTexture(GL_TEXTURE_2D,scene.texture_white);

        // Create the basic gui structure with ImGui
        gui_start_basic_structure(gui,scene, shaders);

        // Perform computation and draw calls for each iteration loop
        exercise.frame_draw(shaders, scene, gui); opengl_debug();


        // Render GUI and update window
        ImGui::End();
        scene.camera_control.update = !(ImGui::IsAnyWindowFocused());
        vcl::imgui_render_frame(gui.window);

        update_fps_title(gui.window, gui.window_title, fps_counter);

        glfwSwapBuffers(gui.window);
        glfwPollEvents();
        opengl_debug();

    }
    std::cout<<"*** Stop GLFW loop ***"<<std::endl;

    // Cleanup ImGui and GLFW
    vcl::imgui_cleanup();

    glfwDestroyWindow(gui.window);
    glfwTerminate();

    return 0;
}

void window_size_callback(GLFWwindow* /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
    scene.camera.perspective.image_aspect = width / static_cast<float>(height);;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    scene.camera_control.update_mouse_move(scene.camera, window, float(xpos), float(ypos));
    exercise.mouse_move(scene, window);
}
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGui::SetWindowFocus(nullptr);
    scene.camera_control.update_mouse_click(scene.camera, window, button, action, mods);
    exercise.mouse_click(scene, window,button,action,mods);
}
