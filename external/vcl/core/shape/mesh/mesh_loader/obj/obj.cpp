#ifdef __linux__
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif 

#include "obj.hpp"

#include "../../../../helper/file/file.hpp"
#include <fstream>
#include <sstream>

namespace vcl
{

mesh mesh_load_file_obj(const std::string& filename)
{
    assert_file_exist(filename);

    mesh m;

    // Open file
    std::ifstream stream(filename);
    assert(stream.is_open());


    while(stream.good())
    {
        std::string buffer;
        std::getline(stream,buffer);

        if( buffer.size()>0 )
        {
            std::stringstream tokens_buffer(buffer);
            std::string first_word;
            tokens_buffer >> first_word;

            if( first_word.size()>0 && first_word[0]!='#' )
            {

                if( first_word=="v" )
                {
                    vec3 p;
                    tokens_buffer >> p.x >> p.y >> p.z;
                    m.position.push_back(p);
                }
                if( first_word=="vt" )
                {
                    vec2 t;
                    tokens_buffer >> t.x >> t.y;
                    m.texture_uv.push_back(t);
                }
                else if(first_word=="f")
                {

                    std::array<std::string,3> s;
                    tokens_buffer >> s[0] >> s[1] >> s[2];

                    index3 f;
                    for(int k=0; k<3; ++k)
                        f[k] = std::stoi(s[k])-1;

                    m.connectivity.push_back(f);
                }
            }
        }
    }



    stream.close();
    assert(!stream.is_open());

    return m;
}



std::vector<vec3> mesh_load_file_obj_read_vertices(const std::string& filename)
{
    assert_file_exist(filename);

    std::vector<vec3> vertices;

    // Open file
    std::ifstream stream(filename);
    assert(stream.is_open());


    while(stream.good())
    {
        std::string buffer;
        std::getline(stream,buffer);

        if( buffer.size()>0 )
        {
            std::stringstream tokens_buffer(buffer);
            std::string first_word;
            tokens_buffer >> first_word;

            if( first_word.size()>0 && first_word[0]!='#' )
            {
                if( first_word=="v" )
                {
                    vec3 p;
                    tokens_buffer >> p.x >> p.y >> p.z;
                    vertices.push_back(p);
                }
            }
        }
    }



    stream.close();
    assert(!stream.is_open());

    return vertices;
}

std::vector<index3> mesh_load_file_obj_read_connectivity(const std::string& filename)
{
    assert_file_exist(filename);

    std::vector<index3> connectivity;

    // Open file
    std::ifstream stream(filename);
    assert(stream.is_open());


    while(stream.good())
    {
        std::string buffer;
        std::getline(stream,buffer);

        if( buffer.size()>0 )
        {
            std::stringstream tokens_buffer(buffer);
            std::string first_word;
            tokens_buffer >> first_word;

            if( first_word.size()>0 && first_word[0]!='#' )
            {
                if(first_word=="f")
                {

                    std::array<std::string,3> s;
                    tokens_buffer >> s[0] >> s[1] >> s[2];

                    index3 f;
                    for(int k=0; k<3; ++k)
                        f[k] = std::stoi(s[k])-1;

                    connectivity.push_back(f);
                }
            }
        }
    }



    stream.close();
    assert(!stream.is_open());

    return connectivity;
}


}
