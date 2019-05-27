
#include "example_animated_hierarchy.hpp"


#ifdef INF443_EXAMPLE_ANIMATED_HIERARCHY


using namespace vcl;



void scene_exercise::setup_data(std::map<std::string,GLuint>& , scene_structure& , gui_structure& )
{
    const float r_body = 0.25f;
    const float r_cylinder = 0.05f;
    const float l_arm = 0.2f;
    mesh body = mesh_primitive_sphere(r_body,{0,0,0},40,40);

    mesh_drawable eye = mesh_primitive_sphere(0.05f,{0,0,0},20,20);
    eye.uniform_parameter.color = {0,0,0};

    mesh arm_top_left = mesh_primitive_cylinder(r_cylinder, {0,0,0}, {-l_arm,0,0});
    mesh arm_bottom_left = mesh_primitive_cylinder(r_cylinder, {0,0,0}, {-l_arm/1.5f,-l_arm/1.0f,0});
    mesh arm_top_right = mesh_primitive_cylinder(r_cylinder, {0,0,0}, {l_arm,0,0});
    mesh arm_bottom_right = mesh_primitive_cylinder(r_cylinder, {0,0,0}, {l_arm/1.5f,-l_arm/1.0f,0});

    mesh shoulder = mesh_primitive_sphere(0.055f);

    hierarchy.add_element(body, "body", "root");
    hierarchy.add_element(eye, "eye_left", "body",{r_body/3,r_body/2,r_body/1.5f});
    hierarchy.add_element(eye, "eye_right", "body",{-r_body/3,r_body/2,r_body/1.5f});

    hierarchy.add_element(arm_top_left, "arm_top_left", "body",{-r_body+0.05f,0,0});
    hierarchy.add_element(arm_bottom_left, "arm_bottom_left", "arm_top_left",{-l_arm,0,0});

    hierarchy.add_element(arm_top_right, "arm_top_right", "body",{r_body-0.05f,0,0});
    hierarchy.add_element(arm_bottom_right, "arm_bottom_right", "arm_top_right",{l_arm,0,0});

    hierarchy.add_element(shoulder, "shoulder_left", "arm_bottom_left");
    hierarchy.add_element(shoulder, "shoulder_right", "arm_bottom_right");

    timer.scale = 0.5f;
}




void scene_exercise::frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& )
{
    timer.update();
    set_gui();

    const float t = timer.t;

    hierarchy.translation("body") = {0,0,0.2f*(1+std::sin(2*3.14f*t))};

    hierarchy.rotation("arm_top_left") = rotation_from_axis_angle_mat3({0,1,0}, std::sin(2*3.14f*(t-0.4f)) );
    hierarchy.rotation("arm_bottom_left") = rotation_from_axis_angle_mat3({0,1,0}, std::sin(2*3.14f*(t-0.6f)) );

    hierarchy.rotation("arm_top_right") = rotation_from_axis_angle_mat3({0,-1,0}, std::sin(2*3.14f*(t-0.4f)) );
    hierarchy.rotation("arm_bottom_right") = rotation_from_axis_angle_mat3({0,-1,0}, std::sin(2*3.14f*(t-0.6f)) );

    hierarchy.draw(shaders["mesh"], scene.camera);
    if(gui_scene.wireframe)
        hierarchy.draw(shaders["wireframe"], scene.camera);

}




void scene_exercise::set_gui()
{
    ImGui::SliderFloat("Time", &timer.t, timer.t_min, timer.t_max);

    const float time_scale_min = 0.1f;
    const float time_scale_max = 3.0f;
    ImGui::SliderFloat("Time scale", &timer.scale, time_scale_min, time_scale_max);
    ImGui::Checkbox("Wireframe", &gui_scene.wireframe);

}



#endif

