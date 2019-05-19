#include "texture_gpu.hpp"

namespace vcl
{

GLuint texture_gpu(const image& im)
{
    GLuint id = 0;
    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_2D,id);

    // Send texture on GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, GLsizei(im.width), GLsizei(im.height), 0, GL_RGBA, GL_UNSIGNED_BYTE, &im.data[0]);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set default texture behavior
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glBindTexture(GL_TEXTURE_2D,0);

    return id;
}



}
