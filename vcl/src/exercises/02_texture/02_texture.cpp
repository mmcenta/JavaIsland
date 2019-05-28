
#include "02_texture.hpp"

#include <random>

#ifdef INF443_02_TEXTURE

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

vcl::mesh create_skybox();


/** This function is called before the beginning of the animation loop
    It is used to initialize all part-specific data */
void scene_exercise::setup_data(std::map<std::string,GLuint>& , scene_structure& scene, gui_structure& )
{



    // setup initial position of the camera
    scene.camera.camera_type = camera_control_spherical_coordinates;
    scene.camera.scale = 10.0f;
    scene.camera.apply_rotation(0,0,0,1.2f);

    terrain = create_terrain();
    terrain.uniform_parameter.color = vec3{1.0f, 1.0f, 1.0f};
    terrain.uniform_parameter.shading.specular = 0;
    texture_terrain = texture_gpu(image_load_png("data/grass.png"));

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




    billboard_surface = create_billboard_surface();
    billboard_surface.uniform_parameter.shading  = {1,0,0};
    texture_flower_billboard = texture_gpu(image_load_png("data/billboard_redflowers.png"));
    texture_grass_billboard = texture_gpu(image_load_png("data/billboard_grass.png"));

    update_grass_position();
    update_flower_position();

    skybox = create_skybox();
    skybox.uniform_parameter.shading = {1,0,0};
    skybox.uniform_parameter.rotation = rotation_from_axis_angle_mat3({1,0,0},-3.014f/2.0f);
    texture_skybox = texture_gpu(image_load_png("data/skybox.png"));


}




/** This function is called at each frame of the animation loop.
    It is used to compute time-varying argument and perform data data drawing */
void scene_exercise::frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& )
{
    set_gui();
    glEnable( GL_POLYGON_OFFSET_FILL ); // avoids z-fighting when displaying wireframe



    display_terrain(shaders, scene);

    display_tree(shaders, scene);
    display_mushroom(shaders, scene);

    display_skybox(shaders, scene);

    display_grass(shaders, scene);
    display_flower(shaders, scene);




}

void scene_exercise::display_terrain(std::map<std::string,GLuint>& shaders, scene_structure& scene)
{
    if(!gui_scene.terrain)
        return ;

    glPolygonOffset( 1.0, 1.0 );
    if(gui_scene.texture_terrain)
        glBindTexture(GL_TEXTURE_2D, texture_terrain);
    terrain.draw(shaders["mesh"], scene.camera);
    glBindTexture(GL_TEXTURE_2D, scene.texture_white);

    if( gui_scene.wireframe_terrain ){
        glPolygonOffset( 1.0, 1.0 );
        terrain.draw(shaders["wireframe"], scene.camera);
    }
}

void scene_exercise::display_tree(std::map<std::string,GLuint>& shaders, scene_structure& scene)
{
    if(!gui_scene.tree)
        return ;

    const vec3 offset_ground = vec3{0,0,-0.025f};
    const size_t N_tree = tree_position.size();
    for(size_t k=0; k<N_tree; ++k)
    {
        const vec3& p = tree_position[k];
        tree_trunc.uniform_parameter.translation = p + offset_ground;
        tree_foliage.uniform_parameter.translation = p + vec3{0,0,0.7f} + offset_ground;


        glPolygonOffset( 1.0, 1.0 );
        tree_trunc.draw(shaders["mesh"], scene.camera);
        tree_foliage.draw(shaders["mesh"], scene.camera);
    }

    if( gui_scene.wireframe ){
        for(size_t k=0; k<N_tree; ++k)
        {
            const vec3& p = tree_position[k];
            tree_trunc.uniform_parameter.translation = p + offset_ground;
            tree_foliage.uniform_parameter.translation = p + vec3{0,0,0.7f} + offset_ground;

            glPolygonOffset( 1.0, 1.0 );
            tree_trunc.draw(shaders["wireframe"], scene.camera);
            tree_foliage.draw(shaders["wireframe"], scene.camera);
        }
    }
}

void scene_exercise::display_mushroom(std::map<std::string,GLuint>& shaders, scene_structure& scene)
{
    if(!gui_scene.mushroom)
        return ;

    const size_t N_mushroom = mushroom_position.size();
    for(size_t k=0; k<N_mushroom; ++k)
    {
        const vec3& p = mushroom_position[k];
        mushroom_trunc.uniform_parameter.translation = p;
        mushroom_top.uniform_parameter.translation = p;

        glPolygonOffset( 1.0, 1.0 );
        mushroom_trunc.draw(shaders["mesh"], scene.camera);
        mushroom_top.draw(shaders["mesh"], scene.camera);
    }

    if( gui_scene.wireframe ){
        for(size_t k=0; k<N_mushroom; ++k)
        {
            const vec3& p = mushroom_position[k];
            mushroom_trunc.uniform_parameter.translation = p;
            mushroom_top.uniform_parameter.translation = p;

            glPolygonOffset( 1.0, 1.0 );
            mushroom_trunc.draw(shaders["wireframe"], scene.camera);
            mushroom_top.draw(shaders["wireframe"], scene.camera);
        }
    }
}

void scene_exercise::display_grass(std::map<std::string,GLuint>& shaders, scene_structure& scene)
{
    if(!gui_scene.grass)
        return;

    glEnable(GL_BLEND);
    glDepthMask(false);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const size_t N_grass = grass_position.size();
    if(gui_scene.texture)
        glBindTexture(GL_TEXTURE_2D, texture_grass_billboard);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    billboard_surface.uniform_parameter.rotation = scene.camera.orientation;
    billboard_surface.uniform_parameter.scaling = 1.5f;

    for(size_t k=0; k<N_grass; ++k)
    {
        const vec3& p = grass_position[k];
        billboard_surface.uniform_parameter.translation = p;

        glPolygonOffset( 1.0, 1.0 );
        billboard_surface.draw(shaders["mesh"], scene.camera);
    }
    glBindTexture(GL_TEXTURE_2D, scene.texture_white);
    glDepthMask(true);


    if( gui_scene.wireframe ){
        for(size_t k=0; k<N_grass; ++k)
        {
            const vec3& p = grass_position[k];
            billboard_surface.uniform_parameter.translation = p;

            glPolygonOffset( 1.0, 1.0 );
            billboard_surface.draw(shaders["wireframe"], scene.camera);
        }
    }


}

void scene_exercise::display_flower(std::map<std::string,GLuint>& shaders, scene_structure& scene)
{
    if(!gui_scene.flower)
        return ;

    glEnable(GL_BLEND);
    glDepthMask(false);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const size_t N_grass = flower_position.size();
    if(gui_scene.texture)
        glBindTexture(GL_TEXTURE_2D, texture_flower_billboard);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    billboard_surface.uniform_parameter.rotation = scene.camera.orientation;
    billboard_surface.uniform_parameter.scaling = 1.0f;
    for(size_t k=0; k<N_grass; ++k)
    {
        const vec3& p = flower_position[k];
        billboard_surface.uniform_parameter.translation = p;

        glPolygonOffset( 1.0, 1.0 );
        billboard_surface.draw(shaders["mesh"], scene.camera);
    }
    glBindTexture(GL_TEXTURE_2D, scene.texture_white);
    glDepthMask(true);


    if( gui_scene.wireframe ){
        for(size_t k=0; k<N_grass; ++k)
        {
            const vec3& p = flower_position[k];
            billboard_surface.uniform_parameter.translation = p;
            glPolygonOffset( 1.0, 1.0 );
            billboard_surface.draw(shaders["wireframe"], scene.camera);
        }
    }


}

void scene_exercise::display_skybox(std::map<std::string,GLuint>& shaders, scene_structure& scene)
{
    if(gui_scene.skybox)
    {
        if(gui_scene.texture)
            glBindTexture(GL_TEXTURE_2D,texture_skybox);
        skybox.uniform_parameter.scaling = 150.0f;
        skybox.uniform_parameter.translation = scene.camera.camera_position() + vec3(0,0,-50.0f);
        skybox.draw(shaders["mesh"], scene.camera);
        glBindTexture(GL_TEXTURE_2D,scene.texture_white);
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

        z += 0.2f*perlin(3*u,3*v, 7, 0.4f);

    }

    return z;
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
    const size_t N = 600;

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N*N);
    terrain.texture_uv.resize(N*N);
    terrain.color.resize(N*N);

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
            terrain.texture_uv[kv+N*ku] = {5*u, 5*v};

            const float c = 0.5f+0.5f*std::max(std::min(terrain.position[kv+N*ku].z/2.0f,1.0f),0.0f);
            terrain.color[kv+N*ku] = {c,c,c,1.0f};
        }
    }


    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
	const unsigned int Ns = N;
	assert(Ns >= 2);
    for(unsigned int ku=0; ku<Ns-1; ++ku)
    {
        for(unsigned int kv=0; kv<Ns-1; ++kv)
        {
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
    const size_t N = 20;
    for(size_t k=0; k<N; ++k)
    {
        const float u = k/float(N);
        const vec3 p = {radius*std::cos(2*3.14f*u), radius*std::sin(2*3.14f*u), 0.0f};
        m.position.push_back( p );
        m.position.push_back( p+vec3(0,0,height) );
    }

	const unsigned int Ns = N;
    for(unsigned int k=0; k<Ns; ++k)
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
    const size_t N = 20;
    for(size_t k=0; k<N; ++k)
    {
        const float u = k/float(N);
        const vec3 p = {radius*std::cos(2*3.14f*u), radius*std::sin(2*3.14f*u), 0.0f};
        m.position.push_back( p+vec3{0,0,z_offset} );
    }
    m.position.push_back({0,0,height+z_offset});

    const unsigned int Ns = N;
    for(unsigned int k=0; k<Ns; ++k) {
        m.connectivity.push_back( {k , (k+1)%Ns, Ns} );
    }

    // closing bottom
    for(size_t k=0; k<N; ++k)
    {
        const float u = k/float(N);
        const vec3 p = {radius*std::cos(2*3.14f*u), radius*std::sin(2*3.14f*u), 0.0f};
        m.position.push_back( p+vec3{0,0,z_offset} );
    }
    m.position.push_back( {0,0,z_offset} );

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

        mushroom_position.push_back(p);
    }
}

vcl::mesh scene_exercise::create_billboard_surface()
{
    mesh billboard;
    billboard.position = {{-0.1f,0,0}, {0.1f,0,0}, {0.1f,0.2f,0}, {-0.1f,0.2f,0}};
    billboard.texture_uv = {{0,1}, {1,1}, {1,0}, {0,0}};
    billboard.connectivity = {{0,1,2}, {0,2,3}};

    return billboard;
}

void scene_exercise::update_grass_position()
{
    const size_t N_grass = 100;

    for(size_t k=0; k<N_grass; ++k)
    {
        const float u = 0.025f+0.95f*distrib(generator);
        const float v = 0.025f+0.95f*distrib(generator);
        const vec3 p = evaluate_terrain(u,v);

        grass_position.push_back(p);
    }
}

void scene_exercise::update_flower_position()
{
    const size_t N_flower = 25;

    for(size_t k=0; k<N_flower; ++k)
    {
        const float u = 0.025f+0.95f*distrib(generator);
        const float v = 0.025f+0.95f*distrib(generator);
        const vec3 p = evaluate_terrain(u,v);

        flower_position.push_back(p);
    }
}

vcl::mesh create_skybox()
{
    const vec3 p000 = {-1,-1,-1};
    const vec3 p001 = {-1,-1, 1};
    const vec3 p010 = {-1, 1,-1};
    const vec3 p011 = {-1, 1, 1};
    const vec3 p100 = { 1,-1,-1};
    const vec3 p101 = { 1,-1, 1};
    const vec3 p110 = { 1, 1,-1};
    const vec3 p111 = { 1, 1, 1};

    mesh skybox;

    skybox.position = {
        p000, p100, p110, p010,
        p010, p110, p111, p011,
        p100, p110, p111, p101,
        p000, p001, p010, p011,
        p001, p101, p111, p011,
        p000, p100, p101, p001
    };


    skybox.connectivity = {
        {0,1,2}, {0,2,3}, {4,5,6}, {4,6,7},
        {8,11,10}, {8,10,9}, {17,16,19}, {17,19,18},
        {23,22,21}, {23,21,20}, {13,12,14}, {13,14,15}
    };

    const float e = 1e-3f;
    const float u0 = 0.0f;
    const float u1 = 0.25f+e;
    const float u2 = 0.5f-e;
    const float u3 = 0.75f-e;
    const float u4 = 1.0f;
    const float v0 = 0.0f;
    const float v1 = 1.0f/3.0f+e;
    const float v2 = 2.0f/3.0f-e;
    const float v3 = 1.0f;
    skybox.texture_uv = {
        {u1,v1}, {u2,v1}, {u2,v2}, {u1,v2},
        {u1,v2}, {u2,v2}, {u2,v3}, {u1,v3},
        {u2,v1}, {u2,v2}, {u3,v2}, {u3,v1},
        {u1,v1}, {u0,v1}, {u1,v2}, {u0,v2},
        {u4,v1}, {u3,v1}, {u3,v2}, {u4,v2},
        {u1,v1}, {u2,v1}, {u2,v0}, {u1,v0}
    };


    return skybox;

}


void scene_exercise::set_gui()
{
    ImGui::Checkbox("Wireframe", &gui_scene.wireframe);
    ImGui::Checkbox("Texture", &gui_scene.texture);

    ImGui::Checkbox("Terrain", &gui_scene.terrain);
    ImGui::Checkbox("Wireframe terrain", &gui_scene.wireframe_terrain);
    ImGui::Checkbox("Texture terrain", &gui_scene.texture_terrain);
    ImGui::Checkbox("Tree", &gui_scene.tree);
    ImGui::Checkbox("Mushroom", &gui_scene.mushroom);
    ImGui::Checkbox("Flower", &gui_scene.flower);
    ImGui::Checkbox("grass", &gui_scene.grass);

    ImGui::Checkbox("Skybox", &gui_scene.skybox);
}

#endif

