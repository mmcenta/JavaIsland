#pragma once

#include "../mesh_drawable/mesh_drawable.hpp"

#include <vector>
#include <map>

namespace vcl{

class mesh_drawable_hierarchy
{
public:

    /** Add an element within the hierarchy
     *
     * visual_param: the mesh to add
     * name: string identifying the element
     * name_parent: name of the parent element
     * translation_local: local translation with respect to the parent element
     * rotation_local: local rotation with respect to the parent element */
    void add_element(const vcl::mesh_drawable& visual_param,
                     const std::string& name,
                     const std::string& name_parent,
                     const vcl::vec3& translation_local = vcl::vec3(0,0,0),
                     const vcl::mat3& rotation_local    = vcl::mat3::identity());


    float scaling = 1.0f;

    int index_of_element(const std::string& name_to_find) const;
    vcl::vec3& translation(const std::string& name);
    vcl::mat3& rotation(const std::string& name);
    vcl::mesh_drawable& mesh_visual(const std::string& name);

    const vcl::vec3& translation(const std::string& name) const;
    const vcl::mat3& rotation(const std::string& name) const;
    const vcl::mesh_drawable& mesh_visual(const std::string& name) const;






    void update_hierarchy();

    void draw(GLuint shader, const vcl::camera_scene& camera);

private:

    std::map<std::string, int> name_map;

    std::vector<std::string> parent_name;

    std::vector<vcl::mesh_drawable> visual;

    std::vector<vcl::vec3> translation_local;
    std::vector<vcl::mat3> rotation_local;

    std::vector<vcl::vec3> translation_global;
    std::vector<vcl::mat3> rotation_global;



};

}
