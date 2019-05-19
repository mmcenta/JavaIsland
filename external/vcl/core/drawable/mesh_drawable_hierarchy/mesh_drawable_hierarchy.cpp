#include "mesh_drawable_hierarchy.hpp"


namespace vcl
{

int mesh_drawable_hierarchy::index_of_element(const std::string& name_to_find) const
{
    const std::map<std::string,int>::const_iterator it = name_map.find(name_to_find);
    if( it==name_map.end() )
    {
        std::cerr<<"Error trying to access element "<<name_to_find.c_str()<<" which is not defined in the hierarchy"<<std::endl;
        return 0;
    }
    else
    {
        return it->second;
    }
}


vcl::vec3& mesh_drawable_hierarchy::translation(const std::string& name)
{
    return translation_local[index_of_element(name)];
}
vcl::mat3& mesh_drawable_hierarchy::rotation(const std::string& name)
{
    return rotation_local[index_of_element(name)];
}
vcl::mesh_drawable& mesh_drawable_hierarchy::mesh_visual(const std::string& name)
{
    return visual[index_of_element(name)];
}

const vcl::vec3& mesh_drawable_hierarchy::translation(const std::string& name) const
{
    return translation_local.at(index_of_element(name));
}
const vcl::mat3& mesh_drawable_hierarchy::rotation(const std::string& name) const
{
    return rotation_local.at(index_of_element(name));
}
const vcl::mesh_drawable& mesh_drawable_hierarchy::mesh_visual(const std::string& name) const
{
    return visual.at(index_of_element(name));
}






void mesh_drawable_hierarchy::add_element(const vcl::mesh_drawable& visual_param,
                 const std::string& name_param,
                 const std::string& name_parent,
                 const vcl::vec3& translation_local_param,
                 const vcl::mat3& rotation_local_param)
{
    const int N = static_cast<int>(name_map.size());
    name_map[name_param] = N;
    parent_name.push_back(name_parent);
    translation_local.push_back(translation_local_param);
    rotation_local.push_back(rotation_local_param);
    visual.push_back(visual_param);
}

void mesh_drawable_hierarchy::update_hierarchy()
{
    if(translation_global.size()!=translation_local.size())
        translation_global.resize(translation_local.size());

    if(rotation_global.size()!=rotation_local.size())
        rotation_global.resize(rotation_local.size());

    const int N = static_cast<int>(translation_global.size());
    assert(N>=0);
    translation_global[0] = translation_local[0];
    rotation_global[0] = rotation_local[0];
    for(int k=1; k<N; ++k)
    {
        const int parent_id = name_map[parent_name[k]];
        rotation_global[k] = rotation_global[parent_id] * rotation_local[k];
        translation_global[k] = scaling * rotation_global[parent_id] * translation_local[k] + translation_global[parent_id];
    }
}

void mesh_drawable_hierarchy::draw(GLuint shader, const vcl::camera_scene& camera)
{
    update_hierarchy();
    size_t N = visual.size();
    for(size_t k=0; k<N; ++k)
    {
        vcl::mesh_drawable& m = visual[k];
        m.uniform_parameter.translation = translation_global[k];
        m.uniform_parameter.rotation = rotation_global[k];
        m.uniform_parameter.scaling = scaling;
        m.draw(shader, camera);
    }
}
}
