#pragma once

#include "vcl/core/shape/mesh/mesh.hpp"
#include "vcl/core/opengl/mesh_gpu/mesh_gpu.hpp"
#include "vcl/core/math/math.hpp"
#include "vcl/core/scene/camera/camera.hpp"
#include "../drawable_uniform/drawable_uniform.hpp"


namespace vcl
{

/** High level OpenGL drawable structure.
 * mesh_drawable contains a mesh_gpu (VAO, VBO), and a set of uniform parameter */
struct mesh_drawable
{
public:

    // Attributes: uniform parameters and mesh_gpu (VAO/VBO)
    drawable_uniform uniform_parameter;
    mesh_gpu data_gpu;

    // Constructors from mesh_gpu or directly from mesh
    mesh_drawable();
    mesh_drawable(const mesh_gpu& data);
    mesh_drawable(const mesh& data);

    /** Display the mesh with and send the uniform parameters */
    void draw(GLuint shader, const camera_scene& camera);

};

}
