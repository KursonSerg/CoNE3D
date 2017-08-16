#ifndef PROGRAM_H
#define PROGRAM_H

#include <unordered_map>

#include <Shaders/Shader.h>

class CProgram
{
public:
    CProgram();
    ~CProgram();

    GLuint getId() const
    {
        return _id;
    }

    void use();
    void unuse();

    void loadShaders(const std::string &filename);
    void loadUniforms();
    void loadUniformBlocks();

    GLint getAttrib(const GLchar *name) const;
    GLint getUniform(const GLchar *name) const;

private:
    GLuint _id;

    void attachShader(const CShader &shader);
    void link();
    void check(GLenum status);

    static const std::unordered_map<std::string, GLint> _textureUnits;
    static GLint getTextureUnit(const std::string &name);

    static const std::unordered_map<std::string, GLuint> _uniformBuffers;
    static GLuint getUniformBufferBinding(const std::string &name);
};

#endif // PROGRAM_H
