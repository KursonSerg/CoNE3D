#include <Shaders/Shader.h>
#include <CLogger.h>

CShader::CShader(GLenum type, const std::string &source) :
    _id(glCreateShader(type))
{
    compile(source);
}

CShader::~CShader()
{
    glDeleteShader(_id);
}

void CShader::compile(const std::string &source)
{
    utils::Log(L"Compiling shader...", utils::ELogLevel::Debug);
    utils::Log(utils::CFormat(L"Shader source:\n%%") << source, utils::ELogLevel::Debug);

    const GLchar *sources[] = { source.c_str() };
    const GLint lengths[] = { static_cast<GLint>(source.size()) };

    glShaderSource(_id, 1, sources, lengths);
    glCompileShader(_id);
    check(GL_COMPILE_STATUS);
}

void CShader::check(GLenum status)
{
    GLint result        = GL_FALSE;
    int   infoLogLength = 0;

    glGetShaderiv(_id, status, &result);

    if (result != GL_TRUE) {
        glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength <= 1) {
            return;
        }

        std::string infoLog(static_cast<size_t>(infoLogLength), 0);
        glGetShaderInfoLog(_id, infoLogLength, nullptr, &infoLog[0]);

        if ( !infoLog.empty() ) {
            throw std::runtime_error( utils::ws2s(utils::CFormat(L"Shader error: %%") << infoLog) );
        }
    }

    utils::checkErrorStatus();
}
