
#include "05_carto.hpp"


#ifdef INF443_05_CARTO

// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::
using namespace vcl;


/** This function is called before the beginning of the animation loop
    It is used to initialize all part-specific data */
void scene_exercise::setup_data(std::map<std::string,GLuint>& , scene_structure& scene, gui_structure& )
{
    // std::vector<polygon> bati = polygon_load_file_shp( "data/IGN/BATI_INDIFFERENCIE.SHP");
    // polygons
    std::vector<polygon> continents = polygon_load_file_shp( "data/GameOfThrones/continents.shp");
    std::vector<polygon> islands = polygon_load_file_shp( "data/GameOfThrones/islands.shp");
    std::vector<polygon> lakes = polygon_load_file_shp( "data/GameOfThrones/lakes.shp");
    std::vector<polygon> landscape = polygon_load_file_shp( "data/GameOfThrones/landscape.shp");
    std::vector<polygon> political = polygon_load_file_shp( "data/GameOfThrones/political.shp");
    std::vector<polygon> regions = polygon_load_file_shp( "data/GameOfThrones/regions.shp");

    // polylines
    std::vector<polygon> rivers = polygon_load_file_shp( "data/GameOfThrones/rivers.shp");
    std::vector<polygon> roads = polygon_load_file_shp( "data/GameOfThrones/roads.shp");
    std::vector<polygon> wall = polygon_load_file_shp( "data/GameOfThrones/wall.shp");

    // points
    locations = points_load_file_shp( "data/GameOfThrones/locations.shp");

    polygons_to_curves(curves, continents, {0,0,0});
    polygons_to_curves(curves, islands, {0.5,0.5,0.1});
    polygons_to_curves(curves, lakes, {0.5,0.5,1});
    polygons_to_curves(curves, landscape, {1.,0.,0.});
    polygons_to_curves(curves, political, {0.5,0.5,1});
    polygons_to_curves(curves, regions, {0.9,0.9,0.9});

    polygons_to_curves(curves, rivers, {0,0,1});
    polygons_to_curves(curves, roads, {0.5,0,0});
    polygons_to_curves(curves, wall, {0.1,0.1,0.1});

    mesh_islands = polygons_to_mesh(islands);
    mesh_islands.uniform_parameter.color = { 0.5,0.5,1. };

    mesh_continents = polygons_to_mesh(continents);
    mesh_continents.uniform_parameter.color = { 1,1,1 };

    // Setup initial camera mode and position
    scene.camera.camera_type = camera_control_spherical_coordinates;
    scene.camera.scale = 10.0f;
    scene.camera.apply_rotation(0,0,0,1.2f);

    cone = mesh_primitive_cone();
}



/** This function is called at each frame of the animation loop.
    It is used to compute time-varying argument and perform data data drawing */
void scene_exercise::frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& )
{
    set_gui();
    glPolygonOffset( 0.0, 0.0 );
    mesh_islands.draw(shaders["mesh"], scene.camera);
    mesh_continents.draw(shaders["mesh"], scene.camera);
    for(unsigned int i = 0; i < locations.size(); ++i) 
    {
        cone.uniform_parameter.translation = locations[i];
        cone.draw(shaders["mesh"], scene.camera);
    }
    glPolygonOffset( 10.0, 10.0 );
    for(vcl::curve_drawable& curve : curves)
    {
        curve.draw(shaders["curve"], scene.camera);
    }

    //mesh.draw(shaders["wireframe"], scene.camera);
}

void scene_exercise::set_gui()
{
}



#endif

