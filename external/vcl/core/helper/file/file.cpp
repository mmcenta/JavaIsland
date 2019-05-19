#include "file.hpp"

#include <fstream>
#include <iostream>
#include <cassert>

namespace vcl
{

void assert_file_exist(const std::string& filename)
{
    // Open file
    std::ifstream stream(filename);

    // Abort program if the file cannot be opened
    if( !stream.is_open() ) {
        std::cerr<<"\nError: cannot access file ["<<filename<<"]"<<std::endl;
        std::cerr<<"=> If the file exists, you may need to adapt the path from which you run the program"<<std::endl;
        exit(1);
    }

    stream.close();
}

std::string read_file_text(const std::string& path)
{
    assert_file_exist(path);

    // Open file with pointer at last position
    std::ifstream stream(path, std::ios::ate);
    assert(stream.is_open());

    std::string full_text;

    // get position of the current pointer (last position)
    std::streampos const s = stream.tellg();

    // resize the buffer to handle the entire text
    full_text.resize( static_cast<size_t>(s) );

    // goes back to the beginning of the file and read the entire text
    stream.seekg(0,std::ios::beg);
    stream.read(&full_text[0],s);

    stream.close();
    assert(!stream.is_open());


    return full_text;

}

}
