#include <CShader.h>
#include <CLogger.h>

#include <vector>

CShader::CShader(const std::string &shader_path, EShaderType shader_type) :
    m_shader_id(0)
{
    // Create the shader
    m_shader_id = glCreateShader( static_cast<GLenum>(shader_type) );

    // Read shader code from the file
    std::string shader_code;
    utils::LoadFile(shader_path, false, shader_code);

    GLint result          = GL_FALSE;
    int   info_log_length = 0;

    // Compile shader
    utils::Log(utils::CFormat(L"Compiling shader '%%'...") << shader_path, utils::ELogLevel::Debug);
    const char *shader_code_pointer = shader_code.c_str();
    glShaderSource(m_shader_id, 1, &shader_code_pointer, nullptr);
    glCompileShader(m_shader_id);

    // Check shader
    glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &result);

    if (result != GL_TRUE) {
        glGetShaderiv(m_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
        if (info_log_length <= 1) {
            return;
        }

        std::string shader_error_message(info_log_length, 0);
        glGetShaderInfoLog(m_shader_id, info_log_length, nullptr, &shader_error_message[0]);

        if ( !shader_error_message.empty() ) {
            throw std::runtime_error( utils::ws2s(utils::CFormat(L"Shader error: %%")
                                                  << shader_error_message) );
        }
    }

    utils::CheckOpenGLError();
}

CShader::~CShader()
{
    glDeleteShader(m_shader_id);
}

CProgram::CProgram() :
    m_program_id(0)
{
    m_program_id = glCreateProgram();
}

CProgram::~CProgram()
{
    glUseProgram(0);
    glDeleteProgram(m_program_id);
}

void CProgram::AttachShader(const CShader &shader)
{
    glAttachShader(m_program_id, shader.m_shader_id);
}

void CProgram::Link()
{
    utils::Log(L"Linking program...", utils::ELogLevel::Debug);
    glLinkProgram(m_program_id);
    Check(GL_LINK_STATUS);
}

void CProgram::Use()
{
    glUseProgram(m_program_id);
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
    GLint result          = GL_FALSE;
    int   info_log_length = 0;

    // Check the program
    glGetProgramiv(m_program_id, status, &result);

    if (result != GL_TRUE) {
        glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &info_log_length);
        if (info_log_length <= 1) {
            return;
        }

        std::string program_error_message(info_log_length, 0);
        glGetProgramInfoLog(m_program_id, info_log_length, nullptr, &program_error_message[0]);

        if ( !program_error_message.empty() ) {
            throw std::runtime_error( utils::ws2s(utils::CFormat(L"Program error: %%")
                                                  << program_error_message) );
        }
    }

    utils::CheckOpenGLError();
}

void CProgram::BindAttrib(GLuint index, const GLchar *name)
{
    glBindAttribLocation(m_program_id, index, name);
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

    GLint attrib = glGetAttribLocation(m_program_id, name);
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

    GLint uniform = glGetUniformLocation(m_program_id, name);
    if (uniform == -1) {
        throw std::runtime_error(std::string("Program uniform not found: ") + name);
    }

    return uniform;
}
