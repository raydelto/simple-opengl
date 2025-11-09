#pragma once

#include <string>
#include <map>
#include "GL/glew.h"
#include "glm/glm.hpp"

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    enum ShaderType
    {
        VERTEX,
        FRAGMENT,
        PROGRAM
    };

    // Only supports vertex and fragment (this series will only have those two)
    bool loadShaders(const char *vsFilename, const char *fsFilename);
    void use();

    GLuint getProgram() const;

    void setUniform(const GLchar *name, const float &f);
    void setUniform(const GLchar *name, const glm::vec2 &v);
    void setUniform(const GLchar *name, const glm::vec3 &v);
    void setUniform(const GLchar *name, const glm::vec4 &v);
    void setUniform(const GLchar *name, const glm::mat4 &m);

    // We are going to speed up looking for uniforms by keeping their locations in a map
    GLint getUniformLocation(const GLchar *name);

private:
    std::string fileToString(const std::string &filename);
    void checkCompileErrors(GLuint shader, ShaderType type);

    GLuint _handle;
    std::map<std::string, GLint> mUniformLocations;
};
