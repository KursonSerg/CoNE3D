#ifndef C_SHADER_H
#define C_SHADER_H

#include <map>

#include <Common.h>

enum class EShaderType {
    Vertex                 = GL_VERTEX_SHADER,
    Fragment               = GL_FRAGMENT_SHADER,
    Geometry               = GL_GEOMETRY_SHADER,
    Compute                = GL_COMPUTE_SHADER,
    TessellationControl    = GL_TESS_CONTROL_SHADER,
    TessellationEvaluation = GL_TESS_EVALUATION_SHADER
};

class CShader
{
public:
    CShader(const std::string &shaderPath, EShaderType shaderType);
    ~CShader();

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
    static void Unuse();
    void Validate();

    void BindAttrib(GLuint index, const GLchar *name);
    typedef std::map<GLuint, std::string> AttribInfo;
    void BindAttrib(AttribInfo attribs);

    GLint GetAttrib(const GLchar *name) const;
    GLint GetUniform(const GLchar *name) const;

private:
    GLuint _programId;

    void Check(GLenum status);
};

#endif // C_SHADER_H
