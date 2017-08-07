#include <Shaders/Program.h>
#include <Shaders/ShaderParser.h>
#include <CLogger.h>

CProgram::CProgram() :
    _id(glCreateProgram())
{
}

CProgram::~CProgram()
{
    glUseProgram(0);
    glDeleteProgram(_id);
}

void CProgram::attachShader(const CShader &shader)
{
    glAttachShader(_id, shader.getId());
}

void CProgram::link()
{
    utils::Log(L"Linking program...", utils::ELogLevel::Debug);
    glLinkProgram(_id);
    check(GL_LINK_STATUS);

#ifndef NDEBUG
    utils::Log(L"Validating program...", utils::ELogLevel::Debug);
    glValidateProgram(_id);
    check(GL_VALIDATE_STATUS);
#endif
}

void CProgram::use()
{
    glUseProgram(_id);
}

void CProgram::unuse()
{
    glUseProgram(0);
}

void CProgram::loadShaders(const std::string &filename)
{
    CShaderParser parser(filename);

    for (const auto &shader : parser.getShaders())
        attachShader(CShader(shader.first, shader.second));

    link();
}

void CProgram::loadUniforms()
{
    use();

    GLint count;
    glGetProgramiv(_id, GL_ACTIVE_UNIFORMS, &count);

    std::stringstream logStream;
    logStream << "Active uniforms: " << count;
    for (GLint i = 0; i < count; ++i)
    {
        GLint size;
        GLenum type;

        const GLsizei MAX_LENGTH = 256;
        GLchar name[MAX_LENGTH];
        GLsizei length;

        glGetActiveUniform(_id, static_cast<GLuint>(i), MAX_LENGTH, &length, &size, &type, name);
        GLint location = glGetUniformLocation(_id, name);

        if (location >= 0 && name[0] == 's')
        {
            // Set all samplers to texture units
            ETextureUnit unit = getTextureUnit(std::string(name).substr(1));
            glUniform1i(location, unit);
        }

        logStream << std::endl << "Uniform #" << i << ": " << name << " (Type: 0x"
                  << std::hex << std::uppercase << type
                  << std::nouppercase << std::dec << ")";
    }
    utils::Log( logStream.str(), utils::ELogLevel::Debug );

    unuse();
}

void CProgram::loadUniformBlocks()
{
    use();

    GLint count;
    glGetProgramiv(_id, GL_ACTIVE_UNIFORM_BLOCKS, &count);

    std::stringstream logStream;
    logStream << "Active uniforms: " << count;
    for (GLint i = 0; i < count; ++i)
    {
        GLint index;
        const GLsizei MAX_LENGTH = 256;
        GLchar name[MAX_LENGTH];
        GLsizei length;

        glGetActiveUniformBlockName(_id, i, MAX_LENGTH, &length, name);
        glGetActiveUniformBlockiv(_id, i,  GL_UNIFORM_BLOCK_BINDING, &index);

        logStream << std::endl << "Uniform #" << i << ": " << name
                  << " (Index: " << index << ")";
    }
    utils::Log( logStream.str(), utils::ELogLevel::Debug );

    unuse();
}

ETextureUnit CProgram::getTextureUnit(const std::string &name)
{
    auto it = _textureUnits.find(name);
    if (it != _textureUnits.end())
        return it->second;
    else
        return MAX_TEXTURE_UNITS;
}

void CProgram::check(GLenum status)
{
    GLint result        = GL_FALSE;
    int   infoLogLength = 0;

    glGetProgramiv(_id, status, &result);

    if (result != GL_TRUE) {
        glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength <= 1) {
            return;
        }

        std::string infoLog(static_cast<size_t>(infoLogLength), 0);
        glGetProgramInfoLog(_id, infoLogLength, nullptr, &infoLog[0]);

        if ( !infoLog.empty() ) {
            throw std::runtime_error( utils::ws2s(utils::CFormat(L"Program error: %%") << infoLog) );
        }
    }

    utils::checkErrorStatus();
}

GLint CProgram::getAttrib(const GLchar *name) const
{
    if (!name) {
        utils::Log(L"Attribute name was NULL", utils::ELogLevel::Warning);
        return -1;
    }

    GLint attrib = glGetAttribLocation(_id, name);
    if (attrib == -1) {
        utils::Log(utils::CFormat(L"Program attribute '%%' not found") << name, utils::ELogLevel::Warning);
    }

    return attrib;
}

GLint CProgram::getUniform(const GLchar *name) const
{
    if (!name) {
        utils::Log(L"Uniform name was NULL", utils::ELogLevel::Warning);
        return -1;
    }

    GLint uniform = glGetUniformLocation(_id, name);
    if (uniform == -1) {
        utils::Log(utils::CFormat(L"Program uniform '%%' not found") << name, utils::ELogLevel::Warning);
    }

    return uniform;
}

const std::unordered_map<std::string, ETextureUnit> CProgram::_textureUnits = {
    { "DiffuseMap", TU_DIFFUSE },
    { "NormalMap", TU_NORMAL },
    { "SpecularMap", TU_SPECULAR },
    { "EmissiveMap", TU_EMISSIVE },
    { "EnvironmentMap", TU_ENVIRONMENT }
};
