#include "lodepng.hpp"

#include <iostream>

namespace vcl
{

image image_load_png(const std::string& filename)
{
    image im;
    unsigned error = lodepng::decode(im.data, im.width, im.height, filename);
    if ( error )
    {
        std::cerr<<"Error Loading png file "<<filename<<std::endl;
        std::cerr<<"Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        exit(1);
    }
    return im;
}

void image_save_png(const std::string& filename, const image& im)
{
    std::vector<unsigned char> output;
    unsigned error = lodepng::encode(filename, im.data, im.width, im.height);
    if ( error )
    {
        std::cerr<<"Error Loading png file "<<filename<<std::endl;
        std::cerr<<"Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        exit(1);
    }
//    else
//        lodepng::save_file(output, filename);

}


}
