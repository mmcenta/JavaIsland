
#include "waves.hpp"

#include <random>

#ifdef WAVES

// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::
using namespace vcl;

// Generator for uniform random number
std::default_random_engine generator;
std::uniform_real_distribution<float> distrib(-1.0,1.0);

mesh create_cylinder(float radius, float height, float offset);
float evaluate_sea_z(float u, float v);
vec3 evaluate_sea(float u, float v);
mesh create_sea();
void update_height();
void update_rays(std::vector<std::vector<oray>>& rays, grid& g, camera_scene& camera);
void update_grid(grid& g, std::vector<std::vector<oray>> rays, plane& p);
float t=0;
float dt = 0.02;
float f_pas = 0.04f;
float f= 0.6;
float threshold = 0.2; //spectrum filtering 
std::vector<vec3> last_positions;
std::vector<vec3> last_normals;


vcl::mesh create_skybox();


/** This function is called before the beginning of the animation loop
    It is used to initialize all part-specific data */
void scene_exercise::setup_data(std::map<std::string,GLuint>& , scene_structure& scene, gui_structure& )
{



    // setup initial position of the camera
    //scene.camera.camera_type = camera_control_spherical_coordinates;
    //scene.camera.scale = 50.0f;
    //scene.camera.apply_rotation(0,0,0,1.2f);
    sea = create_sea();
    sea.uniform_parameter.shading.specular = 0.8;
    sea.uniform_parameter.color= vec3{0,0,0.1}; 



    sky = create_cylinder(140,100,40);
    texture_sun_billboard= texture_gpu(image_load_png("data/sun.png"));
    init_textures();

    billboard_surface = create_billboard_surface(70.0);
    billboard_surface2 = create_billboard_surface(40.0);
    billboard_surface.uniform_parameter.shading  = {1,0,0};
    billboard_surface2.uniform_parameter.shading  = {1,0,0};
    update_cloud_position(130,-10,7,0, 800);
    update_cloud_position(130,7,7,0, 500);
    update_cloud_position(130,13,10,0, 50);
    update_cloud_position(130,16,8,0, 27);
    update_cloud_position(130,80,20,0, 400);
    sun_position= vec3(120*sin(1.50), 120*cos(1.50),3);

    init_spectrum();
    //partie cena

     ocean_plane.point = { 0.0, 0.0, 0.0 };
     ocean_plane.normal = { 0.0, 0.0, 1.0 };

    // Create grid of points
    ogrid.width = 160;
    ogrid.height = 90;
    
    ogrid.points.resize(ogrid.height);
    for (int i = 0; i < ogrid.height; ++i)
        ogrid.points[i].resize(ogrid.width);

    // Create lines
    rays.resize(ogrid.height);
    for (int i = 0; i < ogrid.height; ++i)
        rays[i].resize(ogrid.width);

    // Create mesh to display the segments
    segment_drawer.init();
    segment_drawer.uniform_parameter.color = {0,0,1};

    // Create the sphere to display the points
    const float r = 0.01f;
    sphere = mesh_primitive_sphere(r);
    sphere.uniform_parameter.color = {0.6f, 0.6f, 1.0f};

    // Create the grid representing the ocean
    ocean = mesh_primitive_grid(20, 20, 5.0f, 5.0f, { -2.5f, -2.5f, 0.0f }); 
    ocean.uniform_parameter.color = {0.1f, 0.1f, 1.0f};


}

void scene_exercise::init_textures(){

    for(int i=0; i < 11; i++){
        texture_cloud_billboard[i]= texture_gpu(image_load_png("data/"+to_string(i+1)+".png"));
    }

}

        

void scene_exercise::init_spectrum(){
    float dif =1.0f;
    while(dif> 0.00001){   
        float fp = 0.13*9.81/15;
        dif =0.0081*9.81*9.81* exp(1.2*pow(fp/f,4))/(pow(2*3.1415,4)*pow(f,5));
        std::cout << "dif " << dif << std::endl;
        waves_spectrum.push_back(vec2(dif,2*3.1415*f));
        float var = distrib(generator);
        float var2 = distrib(generator);
        //std::cout << var << std::endl;
        //std::cout << var2 << std::endl;
        waves_directions.push_back(vec2(var, var2)/norm(vec2(var, var2)));
        f+=f_pas;
    }
   // std::cout << waves_spectrum.size();
}

void  scene_exercise::update_height(){

    for(unsigned int i=0;i < last_positions.size(); i++){
        //vec3 pos= sea.data_gpu.vbo_position[i];
        vec2 X0 = {last_positions[i][0],last_positions[i][1]};
        vec2 X = X0;
        float z = last_positions[i][2];
        float z2=0;
        vec2 X2(0,0);
        for(unsigned int j= 0; j < waves_spectrum.size(); j++){
            z+= waves_spectrum[j][0]*cos(waves_spectrum[j][1]*t - dot( waves_directions[j], X));
            z2+= -waves_spectrum[j][0]*waves_spectrum[j][1]*sin(waves_spectrum[j][1]*t - dot( waves_directions[j], X0));
            X+=  waves_directions[j]*waves_spectrum[j][0]*sin(waves_spectrum[j][1]*t - dot( waves_directions[j], X0))/norm(waves_directions[j]);
            X2+=  waves_directions[j]*waves_spectrum[j][0]*cos(waves_spectrum[j][1]*t - dot( waves_directions[j], X0))/norm(waves_directions[j]);
        }
        last_positions[i]= vec3(X[0], X[1],z);
        //std::cout << z << std:: endl;
        const vec3 n(X2[0], X2[1],z2);
        last_normals[i]= n/(norm(n));
        sea.data_gpu.update_position(last_positions);
        sea.data_gpu.update_normal(last_normals);

    }

    t+=dt;
}




/** This function is called at each frame of the animation loop.
    It is used to compute time-varying argument and perform data data drawing */
void scene_exercise::frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& )
{
    set_gui();
    glEnable( GL_POLYGON_OFFSET_FILL ); // avoids z-fighting when displaying wireframe
    
    update_rays(rays, ogrid, scene.camera);
    update_grid(ogrid, rays, ocean_plane);
    ocean.draw(shaders["wireframe"], scene.camera);

    //sea.draw(shaders["mesh"], scene.camera);  
    //update_height();  

    //sky.draw(shaders["wireframe"], scene.camera);
    sky.draw(shaders["ciel"], scene.camera);
    
   
     if(gui_scene.wireframe)
        sea.draw(shaders["wireframe"], scene.camera);

    // Display ocean
    ocean.draw(shaders["wireframe"], scene.camera);

    // Display spheres
    for (int i = 0; i < ogrid.height; ++i) {
        for (int j = 0; j < ogrid.width; ++j) {
            sphere.uniform_parameter.translation = ogrid.points[i][j];       
            sphere.draw(shaders["mesh"], scene.camera);
        }
    }

   
    display_clouds(shaders, scene);
    display_sun(shaders, scene);

}




float evaluate_sea_z(float u, float v)
{
    
    return 0;
}


vec3 evaluate_sea(float u, float v)
{
    const float x = 20*(u-0.5f);
    const float y = 20*(v-0.5f);
    const float z = evaluate_sea_z(u,v);

    return {x,y,z};
}

vcl::mesh scene_exercise::create_billboard_surface(float size)
{
    mesh billboard;
    billboard.position = {{-0.1f*size,0,0}, {0.1f*size,0,0}, {0.1f*size,0.2f*size,0}, {-0.1f*size,0.2f*size,0}};
    billboard.texture_uv = {{0,1}, {1,1}, {1,0}, {0,0}};
    billboard.connectivity = {{0,1,2}, {0,2,3}};

    return billboard;
}

void scene_exercise::update_cloud_position(float radius, float height, float ecart, float ecart2,int  N_clouds)
{

    for(size_t k=0; k<N_clouds; ++k)
    {
        const float height2 = height+ ecart*distrib(generator);
        float var = 2*3.14*distrib(generator);
        float var2 = distrib(generator);
        const float x = (radius+ var2*ecart2)*sin(var);
        const float y = (radius+ var2*ecart2)*cos(var);
        clouds_position.push_back(vec3(x,y,height2));

    }
}

void scene_exercise::display_sun(std::map<std::string,GLuint>& shaders, scene_structure& scene){
            glEnable(GL_BLEND);
            glDepthMask(false);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBindTexture(GL_TEXTURE_2D, texture_sun_billboard);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            billboard_surface2.uniform_parameter.rotation = scene.camera.orientation;
            billboard_surface2.uniform_parameter.scaling = 5.0f;
            billboard_surface2.uniform_parameter.translation = sun_position;

            glPolygonOffset( 1.0, 1.0 );
            billboard_surface2.draw(shaders["mesh"], scene.camera);
            glBindTexture(GL_TEXTURE_2D, scene.texture_white);
            glDepthMask(true);
}

void scene_exercise::display_clouds(std::map<std::string,GLuint>& shaders, scene_structure& scene)
{
    if(!gui_scene.clouds)
        return;

    const size_t N_clouds = clouds_position.size();
    if(gui_scene.cloudstexture){
        for(int i =0; i < 11; i++){
            glEnable(GL_BLEND);
            glDepthMask(false);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBindTexture(GL_TEXTURE_2D, texture_cloud_billboard[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            billboard_surface.uniform_parameter.rotation = scene.camera.orientation;
            billboard_surface.uniform_parameter.scaling = 1.5f;

            for(size_t k=0; k<N_clouds/11; ++k)
            {
                const vec3& p = clouds_position[i*N_clouds/12+k];
                billboard_surface.uniform_parameter.translation = p;

                glPolygonOffset( 1.0, 1.0 );
                billboard_surface.draw(shaders["mesh"], scene.camera);
            }

            
            glBindTexture(GL_TEXTURE_2D, scene.texture_white);
            glDepthMask(true);
        }
    }
    

    if( gui_scene.wireframeclouds ){
        for(size_t k=0; k<N_clouds; ++k)
        {
            const vec3& p = clouds_position[k];
            billboard_surface.uniform_parameter.translation = p;

            glPolygonOffset( 1.0, 1.0 );
            billboard_surface.draw(shaders["wireframe"], scene.camera);
        }
    }


}



mesh create_cylinder(float radius, float height, float offset)
{
    mesh m;

    // Number of samples
    const size_t N = 20;

    // Geometry
    for(size_t k=0; k<N; ++k)
    {
        const float u = k/float(N);
        const vec3 p = {radius*std::cos(2*3.14f*u), radius*std::sin(2*3.14f*u), 0.0f};
        m.position.push_back( p+vec3(0,0,-offset) );
        m.color.push_back(vec4(4,4,4,1));
        m.position.push_back( p+vec3(0,0,height) );
        m.color.push_back(vec4(0,2,4,1));
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


mesh create_sea()
{
    // Number of samples of the terrain is N x N
    const size_t N = 100;

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N*N);
    terrain.texture_uv.resize(N*N);
    terrain.color.resize(N*N);
    terrain.normal.resize(N*N);
    last_positions.resize(N*N);
    last_normals.resize(N*N);

    // Fill terrain geometry
    for(size_t ku=0; ku<N; ++ku)
    {
        for(size_t kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku/(N-1.0f);
            const float v = kv/(N-1.0f);

            // Compute coordinates
            terrain.position[kv+N*ku] = evaluate_sea(u,v);
            last_positions[kv+N*ku]= evaluate_sea(u,v);
            terrain.normal[kv+N*ku]= vec3(0,0,1);
            last_normals[kv+N*ku]= vec3(0,0,1);
            terrain.color[kv+N*ku] = vec4{0.7f,0.4f,2.6f, 1.0f};

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



void update_pas(const gui_scene_structure &gui_scene){
    dt = gui_scene.scaling;
}

void update_thre(const gui_scene_structure &gui_scene){
    threshold = gui_scene.threshold;
}




void scene_exercise::set_gui()
{
    ImGui::Checkbox("Wireframe", &gui_scene.wireframe);
    ImGui::Checkbox("Clouds", &gui_scene.clouds);
    ImGui::Checkbox("Cloud Texture", &gui_scene.cloudstexture);
    ImGui::Checkbox("Wire Frame cloulds", &gui_scene.wireframeclouds);
    float scaling_min = 0.01f;
    float scaling_max = 0.8f;
    if( ImGui::SliderScalar("dt", ImGuiDataType_Float, &gui_scene.scaling, &scaling_min, &scaling_max) )
        update_pas(gui_scene);
    float threshold_min = 0.01f;
    float threshold_max = 0.8f;
    if( ImGui::SliderScalar("threshold", ImGuiDataType_Float, &gui_scene.threshold, &threshold_min, &threshold_max) )
        update_thre(gui_scene);


}

void update_rays(std::vector<std::vector<oray>>& rays, grid& g, camera_scene& camera) {
    const mat4 R = camera.camera_matrix();
    const vec3 p0 = camera.camera_position();
    const float d = 1/std::tan(camera.perspective.angle_of_view/2);
    const float aspect_ratio = camera.perspective.image_aspect;

    const float w_step = 2.0/g.width;
    const float h_step = 2.0/g.height;

    for (int i = 0; i < g.height; ++i) {
        for (int j = 0; j < g.width; ++j) {
            vec4 p_grid = {(-1.0f + (j + 0.5f)*w_step)*aspect_ratio,
                                           1.0f - (i + 0.5f)*h_step,
                                                                 -d,
                                                               1.0f };
            p_grid = R*p_grid; 
            
            const vec3 p1 = { p_grid[0]/p_grid[3],
                              p_grid[1]/p_grid[3],
                              p_grid[2]/p_grid[3] };
            const vec3 dir = normalize(p1-p0);

            rays[i][j].origin = p0;
            rays[i][j].direction = dir;
        }
    }
}

vec3 intersect_ray_plane(const oray& l, const plane& p) {
    const float dot_prod = dot(l.direction, p.normal);

    if (dot_prod != 0) {
        const float d = dot(p.point - l.origin, p.normal)/dot_prod;   

        return (l.origin + d*l.direction);
    }

    return { 0.0, 0.0, 0.0};
}

void update_grid(grid& g, std::vector<std::vector<oray>> rays, plane& p) {
    for (int i = 0; i < g.height; ++i)
        for (int j = 0; j < g.width; ++j)
            g.points[i][j] = intersect_ray_plane(rays[i][j], p);
}

#endif

