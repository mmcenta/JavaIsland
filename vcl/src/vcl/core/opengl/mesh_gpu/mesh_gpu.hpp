#pragma once

#include "vcl/external_lib/headers/glad.hpp"
#include "vcl/core/shape/mesh/mesh.hpp"


namespace vcl
{

/** OpenGL drawable mesh structure. mesh_gpu stores a set of VBO and a VAO initialized from a mesh structure */
struct mesh_gpu
{
    mesh_gpu();
    /** Initialize VAO and VBO from the mesh */
    mesh_gpu(const mesh& mesh_cpu);

    /** Clear buffers */
    void clear();

    /** Dynamically update the VBO with the new vector of position
     * Warning: new_position is excpected to have the same size (or less) than the initialized one */
    void update_position(const std::vector<vec3>& new_position);

    /** Dynamically update the VBO with the new vector of normal
     * Warning: new_normal is excpected to have the same size (or less) than the initialized one */
    void update_normal(const std::vector<vec3>& new_normal);

    /** Attributes of the mesh_gpu: VAO and VBO as well as the number of triangle */
    GLuint vao;
    GLuint vbo_index;

    GLuint vbo_position;
    GLuint vbo_normal;
    GLuint vbo_color;
    GLuint vbo_texture_uv;

    unsigned int number_triangles;



};

/** Call OpenGL draw */
void draw(const mesh_gpu& surface);





}
