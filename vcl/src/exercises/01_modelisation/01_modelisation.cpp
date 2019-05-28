
#include "01_modelisation.hpp"

#include <random>

#ifdef INF443_01_MODELISATION

// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::
using namespace vcl;

// Generator for uniform random number
std::default_random_engine generator;
std::uniform_real_distribution<float> distrib(0.0,1.0);


float evaluate_terrain_z(float u, float v);
vec3 evaluate_terrain(float u, float v);
mesh create_terrain();

mesh create_cylinder(float radius, float height);
mesh create_cone(float radius, float height, float z_offset);

mesh create_tree_foliage(float radius, float height, float z_offset);



/** This function is called before the beginning of the animation loop
    It is used to initialize all part-specific data */
void scene_exercise::setup_data(std::map<std::string,GLuint>& , scene_structure& scene, gui_structure& )
{
    // setup initial position of the camera
    scene.camera.camera_type = camera_control_spherical_coordinates;
    scene.camera.scale = 10.0f;
    scene.camera.apply_rotation(0,0,0,1.2f);

    terrain = create_terrain();
    terrain.uniform_parameter.color = vec3{0.8f,1.0f,0.8f};
    terrain.uniform_parameter.shading.specular = 0;

    tree_trunc = create_cylinder(0.1f, 0.7f);
    tree_trunc.uniform_parameter.color = {0.4f, 0.3f, 0.3f};

    tree_foliage = create_tree_foliage(0.4f, 0.6f, 0.2f);
    tree_foliage.uniform_parameter.translation = {0,0,0.7f};
    tree_foliage.uniform_parameter.color = {0.4f, 0.6f, 0.3f};

    update_tree_position();



    mushroom_trunc = create_cylinder(0.03f, 0.1f);
    mushroom_trunc.uniform_parameter.color = {0.4f, 0.4f, 0.4f};
    mushroom_top = create_cone(0.1f, 0.06f, 0.1f);
    mushroom_top.uniform_parameter.color = {0.8f, 0.1f, 0.1f};
    update_mushroom_position();

    texture_id = texture_gpu( image_load_png("data/grass.png") );

}




/** This function is called at each frame of the animation loop.
    It is used to compute time-varying argument and perform data data drawing */
void scene_exercise::frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& )
{
    set_gui();

    glEnable( GL_POLYGON_OFFSET_FILL ); // avoids z-fighting when displaying wireframe


    // Display terrain
    glPolygonOffset( 1.0, 1.0 );
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    terrain.draw(shaders["mesh"], scene.camera);

    if( gui_scene.wireframe ){
        glPolygonOffset( 1.0, 1.0 );
        terrain.draw(shaders["wireframe"], scene.camera);
    }

    display_tree(shaders, scene);
    display_mushroom(shaders, scene);

}

void scene_exercise::display_tree(std::map<std::string,GLuint>& shaders, scene_structure& scene)
{
    const vec3 offset_ground = vec3{0,0,-0.025f};
    const size_t N_tree = tree_position.size();
    for(size_t k=0; k<N_tree; ++k)
    {
        const vec3& p = tree_position[k];
        tree_trunc.uniform_parameter.translation = p + offset_ground;
        tree_foliage.uniform_parameter.translation = p + vec3{0,0,0.7f} + offset_ground;

        tree_trunc.draw(shaders["mesh"], scene.camera);
        tree_foliage.draw(shaders["mesh"], scene.camera);
    }

    if( gui_scene.wireframe ){
        for(size_t k=0; k<N_tree; ++k)
        {
            const vec3& p = tree_position[k];
            tree_trunc.uniform_parameter.translation = p + offset_ground;
            tree_foliage.uniform_parameter.translation = p + vec3{0,0,0.7f} + offset_ground;

            tree_trunc.draw(shaders["wireframe"], scene.camera);
            tree_foliage.draw(shaders["wireframe"], scene.camera);
        }
    }
}

void scene_exercise::display_mushroom(std::map<std::string,GLuint>& shaders, scene_structure& scene)
{
    const size_t N_mushroom = mushroom_position.size();
    for(size_t k=0; k<N_mushroom; ++k)
    {
        const vec3& p = mushroom_position[k];
        mushroom_trunc.uniform_parameter.translation = p;
        mushroom_top.uniform_parameter.translation = p;

        mushroom_trunc.draw(shaders["mesh"], scene.camera);
        mushroom_top.draw(shaders["mesh"], scene.camera);
    }

    if( gui_scene.wireframe ){
        for(size_t k=0; k<N_mushroom; ++k)
        {
            const vec3& p = mushroom_position[k];
            mushroom_trunc.uniform_parameter.translation = p;
            mushroom_top.uniform_parameter.translation = p;

            mushroom_trunc.draw(shaders["wireframe"], scene.camera);
            mushroom_top.draw(shaders["wireframe"], scene.camera);
        }
    }
}



float evaluate_terrain_z(float u, float v)
{
    const std::vector<vec2> pi = {{0,0}, {0.5f,0.5f}, {0.2f,0.7f}, {0.8f,0.7f}};
    const std::vector<float> hi = {3.0f, -1.5f, 1.0f, 2.0f};
    const std::vector<float> sigma_i = {0.5f, 0.15f, 0.2f, 0.2f};

    const size_t N = pi.size();
    float z = 0.0f;
    for(size_t k=0; k<N; ++k)
    {
        const float u0 = pi[k].x;
        const float v0 = pi[k].y;
        const float d2 = (u-u0)*(u-u0)+(v-v0)*(v-v0);
        z += hi[k] * std::exp( - d2/sigma_i[k]/sigma_i[k] );

    }

    const float scaling = 3.0;
    const int octave = 7;
    const float persistency = 0.4;

    const float noise = perlin(scaling*u, scaling*v, octave, persistency);
    return z*noise;
}


vec3 evaluate_terrain(float u, float v)
{
    const float x = 20*(u-0.5f);
    const float y = 20*(v-0.5f);
    const float z = evaluate_terrain_z(u,v);

    return {x,y,z};
}


mesh create_terrain()
{
    // Number of samples of the terrain is N x N
    const size_t N = 100;

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N*N);

    // Fill terrain geometry
    for(size_t ku=0; ku<N; ++ku)
    {
        for(size_t kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku/(N-1.0f);
            const float v = kv/(N-1.0f);

            // Compute coordinates
            terrain.position[kv+N*ku] = evaluate_terrain(u,v);
            terrain.texture_uv.push_back({u,v});
        }
    }


    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for(size_t ku=0; ku<N-1; ++ku)
    {
        for(size_t kv=0; kv<N-1; ++kv)
        {
            const unsigned int Ns = N;
            const unsigned int idx = kv + Ns*ku; // current vertex offset

            const index3 triangle_1 = {idx, idx+1+Ns, idx+1};
            const index3 triangle_2 = {idx, idx+Ns, idx+1+Ns};

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    return terrain;
}



mesh create_cylinder(float radius, float height)
{
    mesh m;

    // Number of samples
    const size_t N = 20;

    // Geometry
    for(size_t k=0; k<N; ++k)
    {
        const float u = k/float(N);
        const vec3 p = {radius*std::cos(2*3.14f*u), radius*std::sin(2*3.14f*u), 0.0f};
        m.position.push_back( p );
        m.position.push_back( p+vec3(0,0,height) );
    }

    // Connectivity
    for(size_t k=0; k<N; ++k)
    {
        const unsigned int u00 = 2*k;
        const unsigned int u01 = (2*k+1)%(2*N);
        const unsigned int u10 = (2*(k+1))%(2*N);
        const unsigned int u11 = (2*(k+1)+1) % (2*N);

        const index3 t1 = {u00, u10, u11};
        const index3 t2 = {u00, u11, u01};
        m.connectivity.push_back(t1);
        m.connectivity.push_back(t2);
    }

    return m;
}



mesh create_cone(float radius, float height, float z_offset)
{
    mesh m;

    // conical structure
    // *************************** //

    const size_t N = 20;

    // geometry
    for(size_t k=0; k<N; ++k)
    {
        const float u = k/float(N);
        const vec3 p = {radius*std::cos(2*3.14f*u), radius*std::sin(2*3.14f*u), 0.0f};
        m.position.push_back( p+vec3{0,0,z_offset} );
    }
    // apex
    m.position.push_back({0,0,height+z_offset});

    // connectivity
    const unsigned int Ns = N;
    for(unsigned int k=0; k<Ns; ++k) {
        m.connectivity.push_back( {k , (k+1)%Ns, Ns} );
    }

    // close the bottom of the cone
    // *************************** //

    // Geometry
    for(size_t k=0; k<N; ++k)
    {
        const float u = k/float(N);
        const vec3 p = {radius*std::cos(2*3.14f*u), radius*std::sin(2*3.14f*u), 0.0f};
        m.position.push_back( p+vec3{0,0,z_offset} );
    }
    // central position
    m.position.push_back( {0,0,z_offset} );

    // connectivity
    for(unsigned int k=0; k<Ns; ++k)
        m.connectivity.push_back( {k+Ns+1, (k+1)%Ns+Ns+1, 2*Ns+1} );

    return m;
}

mesh create_tree_foliage(float radius, float height, float z_offset)
{
    mesh m = create_cone(radius, height, 0);
    m.push_back( create_cone(radius, height, z_offset) );
    m.push_back( create_cone(radius, height, 2*z_offset) );

    return m;
}

void scene_exercise::update_tree_position()
{
    const size_t N_tree = 50;

    for(size_t k=0; k<N_tree; ++k)
    {
        const float u = 0.025f+0.95f*distrib(generator);
        const float v = 0.025f+0.95f*distrib(generator);
        const vec3 p = evaluate_terrain(u,v);

        const float r_min = 0.8f;
        bool to_add=true;
        for(size_t k_test=0; to_add==true && k_test<tree_position.size(); ++k_test)
        {
            const vec3& p0 = tree_position[k_test];
            if(norm(p-p0)<r_min)
                to_add=false;
        }
        if( to_add==true)
            tree_position.push_back(p);
    }
}

void scene_exercise::update_mushroom_position()
{
    const size_t N_mushroom = 50;

    for(size_t k=0; k<N_mushroom; ++k)
    {
        const float u = 0.025f+0.95f*distrib(generator);
        const float v = 0.025f+0.95f*distrib(generator);
        const vec3 p = evaluate_terrain(u,v);

        const float r_min = 0.2f;
        bool to_add=true;
        for(size_t k_test=0; to_add==true && k_test<mushroom_position.size(); ++k_test)
        {
            const vec3& p0 = mushroom_position[k_test];
            if(norm(p-p0)<r_min)
                to_add=false;
        }
        if( to_add==true)
            mushroom_position.push_back(p);
    }
}



void scene_exercise::set_gui()
{
    ImGui::Checkbox("Wireframe", &gui_scene.wireframe);
}

#endif

