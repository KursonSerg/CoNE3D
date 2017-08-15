#ifndef PROGRAM_H
#define PROGRAM_H

#include <unordered_map>

#include <Shaders/Shader.h>

enum ETextureUnit
{
    TU_DIFFUSE = 0,
    TU_NORMAL = 1,
    TU_SPECULAR = 2,
    TU_EMISSIVE = 3,
    TU_ENVIRONMENT = 4,

    MAX_TEXTURE_UNITS
};

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

    static const std::unordered_map<std::string, ETextureUnit> _textureUnits;
    static ETextureUnit getTextureUnit(const std::string &name);
};

#endif // PROGRAM_H
