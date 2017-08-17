#include <Shaders/Program.h>
#include <Shaders/ShaderParser.h>
#include <CLogger.h>

#include <vector>
#include <Bindings.glsl>

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
        const GLsizei MAX_LENGTH = 256;
        GLchar name[MAX_LENGTH];
        GLsizei length;

        GLint size;
        GLenum type;
        glGetActiveUniform(_id, static_cast<GLuint>(i), MAX_LENGTH, &length, &size, &type, name);
        GLint location = glGetUniformLocation(_id, name);

        if (location >= 0 && name[0] == 's')
        {
            // Set all samplers to texture units
            GLint unit = getTextureUnit(std::string(name, length).substr(1));
            if (unit != MAX_TEXTURE_UNITS)
                glUniform1i(location, unit);
        }

#ifndef NDEBUG
        logStream << std::endl << "Uniform #" << i << ": " << name << " (Type: 0x"
                  << std::hex << std::uppercase << type
                  << std::nouppercase << std::dec << ")";
#endif
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
    logStream << "Active uniform blocks: " << count;
    for (GLint i = 0; i < count; ++i)
    {
        const GLsizei MAX_LENGTH = 256;
        GLchar name[MAX_LENGTH];
        GLsizei length;

        glGetActiveUniformBlockName(_id, i, MAX_LENGTH, &length, name);
        if (length > 0)
        {
            // Bind all uniform buffer objects
            GLuint binding = getUniformBufferBinding(std::string(name, length));
            if (binding != GL_INVALID_INDEX)
                glUniformBlockBinding(_id, i, binding);
        }

#ifndef NDEBUG
        GLint binding, size, uniformsNumber;
        glGetActiveUniformBlockiv(_id, i,  GL_UNIFORM_BLOCK_BINDING, &binding);
        glGetActiveUniformBlockiv(_id, i, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
        glGetActiveUniformBlockiv(_id, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniformsNumber);

        std::vector<GLint> indices(uniformsNumber);
        glGetActiveUniformBlockiv(_id, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices.data());

        std::vector<GLint> offsets(uniformsNumber);
        glGetActiveUniformsiv(_id, uniformsNumber, reinterpret_cast<GLuint *>(indices.data()), GL_UNIFORM_OFFSET, offsets.data());

        logStream << std::endl << "Uniform block #" << i << ": " << name
                  << " (binding: " << binding
                  << ", size: " << size
                  << ", uniforms: " << uniformsNumber << ")";

        for (int j = 0; j < uniformsNumber; ++j)
        {
            glGetActiveUniformName(_id, indices[j], MAX_LENGTH, &length, name);
            logStream << std::endl << "  Uniform #" << j << ": " << name
                      << " (index: " << indices[j]
                      << ", offset: " << offsets[j] << ")";
        }
#endif
    }
    utils::Log( logStream.str(), utils::ELogLevel::Debug );

    unuse();
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

GLint CProgram::getTextureUnit(const std::string &name)
{
    auto it = _textureUnits.find(name);
    if (it != _textureUnits.end()) {
        return it->second;
    } else {
        utils::Log(utils::CFormat(L"Unknown '%%' texture unit") << name, utils::ELogLevel::Warning);
        return MAX_TEXTURE_UNITS;
    }
}

GLuint CProgram::getUniformBufferBinding(const std::string &name)
{
    auto it = _uniformBuffers.find(name);
    if (it != _uniformBuffers.end()) {
        return it->second;
    } else {
        utils::Log(utils::CFormat(L"Unknown '%%' uniform buffer") << name, utils::ELogLevel::Warning);
        return GL_INVALID_INDEX;
    }
}

const std::unordered_map<std::string, GLint> CProgram::_textureUnits = {
    { "DiffuseMap", TEXTURE_UNIT_DIFFUSE },
    { "NormalMap", TEXTURE_UNIT_NORMAL },
    { "SpecularMap", TEXTURE_UNIT_SPECULAR },
    { "EmissiveMap", TEXTURE_UNIT_EMISSIVE }
};

const std::unordered_map<std::string, GLuint> CProgram::_uniformBuffers = {
    { "Camera", BUFFER_CAMERA_BINDING },
    { "Material", BUFFER_MATERIAL_BINDING },
    { "Transform", BUFFER_TRANSFORM_BINDING },
    { "Light", BUFFER_LIGHT_BINDING }
};
