#pragma once

#include "vcl/core/math/math.hpp"
#include "vcl/external_lib/headers/glad.hpp"

#include <string>


namespace vcl
{

void uniform(GLuint shader, const std::string& name, const float value);
void uniform(GLuint shader, const std::string& name, const vec3& value);
void uniform(GLuint shader, const std::string& name, const vec4& value);
void uniform(GLuint shader, const std::string& name, float x, float y, float z);
void uniform(GLuint shader, const std::string& name, float x, float y, float z, float w);
void uniform(GLuint shader, const std::string& name, const mat4& m);
void uniform(GLuint shader, const std::string& name, const mat3& m);

}
