#include <CShader.h>
#include <CLogger.h>

#include <vector>

CShader::CShader(EShaderType shaderType, const std::string &shaderPath) :
    _shaderId(0)
{
    _shaderId = glCreateShader( static_cast<GLenum>(shaderType) );

    Compile(shaderPath);
}

CShader::~CShader()
{
    glDeleteShader(_shaderId);
}

void CShader::Compile(const std::string &shaderPath)
{
    if (shaderPath.empty())
        return;

    std::string shaderCode;
    utils::LoadFile(shaderPath, false, shaderCode);

    utils::Log(utils::CFormat(L"Compiling shader '%%'...") << shaderPath, utils::ELogLevel::Debug);
    const char *shaderCodePointer = shaderCode.c_str();
    glShaderSource(_shaderId, 1, &shaderCodePointer, nullptr);
    glCompileShader(_shaderId);

    Check(GL_COMPILE_STATUS);
}

void CShader::Check(GLenum status)
{
    GLint result        = GL_FALSE;
    int   infoLogLength = 0;

    glGetShaderiv(_shaderId, status, &result);

    if (result != GL_TRUE) {
        glGetShaderiv(_shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength <= 1) {
            return;
        }

        std::string infoLog(static_cast<size_t>(infoLogLength), 0);
        glGetShaderInfoLog(_shaderId, infoLogLength, nullptr, &infoLog[0]);

        if ( !infoLog.empty() ) {
            throw std::runtime_error( utils::ws2s(utils::CFormat(L"Shader error: %%") << infoLog) );
        }
    }

    utils::CheckOpenGLError();
}

CProgram::CProgram() :
    _programId(0)
{
    _programId = glCreateProgram();
}

CProgram::~CProgram()
{
    glUseProgram(0);
    glDeleteProgram(_programId);
}

void CProgram::AttachShader(const CShader &shader)
{
    glAttachShader(_programId, shader._shaderId);
}

void CProgram::Link()
{
    utils::Log(L"Linking program...", utils::ELogLevel::Debug);
    glLinkProgram(_programId);
    Check(GL_LINK_STATUS);
}

void CProgram::Use()
{
    glUseProgram(_programId);
}

void CProgram::Unuse()
{
    glUseProgram(0);
}

void CProgram::Validate()
{
    utils::Log(L"Validating program...", utils::ELogLevel::Debug);
    Check(GL_VALIDATE_STATUS);
}

void CProgram::Check(GLenum status)
{
    GLint result        = GL_FALSE;
    int   infoLogLength = 0;

    glGetProgramiv(_programId, status, &result);

    if (result != GL_TRUE) {
        glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength <= 1) {
            return;
        }

        std::string infoLog(static_cast<size_t>(infoLogLength), 0);
        glGetProgramInfoLog(_programId, infoLogLength, nullptr, &infoLog[0]);

        if ( !infoLog.empty() ) {
            throw std::runtime_error( utils::ws2s(utils::CFormat(L"Program error: %%") << infoLog) );
        }
    }

    utils::CheckOpenGLError();
}

void CProgram::BindAttrib(GLuint index, const GLchar *name)
{
    glBindAttribLocation(_programId, index, name);
}

void CProgram::BindAttrib(AttribInfo attribs)
{
    for (const auto &attrib : attribs)
        BindAttrib(attrib.first, attrib.second.c_str());
}

GLint CProgram::GetAttrib(const GLchar *name) const
{
    if (!name) {
        throw std::runtime_error("Attribute name was NULL");
    }

    GLint attrib = glGetAttribLocation(_programId, name);
    if (attrib == -1) {
        throw std::runtime_error(std::string("Program attribute not found: ") + name);
    }

    return attrib;
}

GLint CProgram::GetUniform(const GLchar *name) const
{
    if (!name) {
        throw std::runtime_error("Uniform name was NULL");
    }

    GLint uniform = glGetUniformLocation(_programId, name);
    if (uniform == -1) {
        throw std::runtime_error(std::string("Program uniform not found: ") + name);
    }

    return uniform;
}
