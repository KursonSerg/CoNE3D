#ifndef C_SHADER_H
#define C_SHADER_H

#include <unordered_map>

#include <Common.h>

enum class EShaderType {
    Vertex                 = GL_VERTEX_SHADER,
    Fragment               = GL_FRAGMENT_SHADER,
    Geometry               = GL_GEOMETRY_SHADER,
    Compute                = GL_COMPUTE_SHADER,
    TessellationControl    = GL_TESS_CONTROL_SHADER,
    TessellationEvaluation = GL_TESS_EVALUATION_SHADER
};

enum ETextureUnit
{
    TU_DIFFUSE = 0,
    TU_NORMAL = 1,
    TU_SPECULAR = 2,
    TU_EMISSIVE = 3,
    TU_ENVIRONMENT = 4,

    MAX_TEXTURE_UNITS
};

class CShader
{
public:
    CShader(EShaderType shaderType, const std::string &shaderPath = std::string());
    ~CShader();

    void Compile(const std::string &shaderPath);

private:
    GLuint _shaderId;
    void Check(GLenum status);

    friend class CProgram;
};

class CProgram
{
public:
    CProgram();
    ~CProgram();

    void AttachShader(const CShader &shader);
    void Link();
    void Use();
    void Unuse();
    void Validate();
    void LoadUniforms();

    GLint GetAttrib(const GLchar *name) const;
    GLint GetUniform(const GLchar *name) const;

private:
    GLuint _programId;
    void Check(GLenum status);

    static const std::unordered_map<std::string, ETextureUnit> _textureUnits;
    static ETextureUnit GetTextureUnit(const std::string &name);
};

#endif // C_SHADER_H
