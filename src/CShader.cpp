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

    utils::checkErrorStatus();
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

void CProgram::LoadUniforms()
{
    Use();

    GLint count;
    glGetProgramiv(_programId, GL_ACTIVE_UNIFORMS, &count);

    std::stringstream log_stream;
    log_stream << "Active uniforms: " << count << std::endl;
    for (GLint i = 0, unit = 0; i < count; ++i)
    {
        GLint size;
        GLenum type;

        const GLsizei maxLength = 64;
        GLchar name[maxLength];
        GLsizei length;

        glGetActiveUniform(_programId, static_cast<GLuint>(i), maxLength, &length, &size, &type, name);

        switch (type) {
        case GL_SAMPLER_1D:
        case GL_SAMPLER_2D:
        case GL_SAMPLER_3D:
        case GL_SAMPLER_CUBE:
        case GL_SAMPLER_1D_SHADOW:
        case GL_SAMPLER_2D_SHADOW:
            // Set all texture samplers to texture units in order of declaration
            glUniform1i(i, unit++);
            break;
        }

        log_stream << "Uniform #" << i << ": " << name << " (Type: 0x"
                   << std::hex << std::uppercase << type
                   << std::nouppercase << std::dec << ")" << std::endl;
    }
    utils::Log( log_stream.str(), utils::ELogLevel::Debug );

    Unuse();
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

    utils::checkErrorStatus();
}

GLint CProgram::GetAttrib(const GLchar *name) const
{
    if (!name) {
        utils::Log(L"Attribute name was NULL", utils::ELogLevel::Warning);
        return -1;
    }

    GLint attrib = glGetAttribLocation(_programId, name);
    if (attrib == -1) {
        utils::Log(utils::CFormat(L"Program attribute '%%' not found") << name, utils::ELogLevel::Warning);
    }

    return attrib;
}

GLint CProgram::GetUniform(const GLchar *name) const
{
    if (!name) {
        utils::Log(L"Uniform name was NULL", utils::ELogLevel::Warning);
        return -1;
    }

    GLint uniform = glGetUniformLocation(_programId, name);
    if (uniform == -1) {
        utils::Log(utils::CFormat(L"Program uniform '%%' not found") << name, utils::ELogLevel::Warning);
    }

    return uniform;
}
