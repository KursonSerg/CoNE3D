#include <Common.h>
#include <CLogger.h>

namespace utils {
// http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
size_t LoadFile(const std::wstring &filename, bool binary, std::wstring &buffer)
{
    std::wifstream in(utils::ws2s(filename),
                      binary ? std::ios::in | std::ios::binary : std::ios::in);

    if ( in.is_open() ) {
        in.seekg(0, std::ios::end);
        buffer.clear();
        buffer.resize( in.tellg() );
        in.seekg(0, std::ios::beg);

        in.read( &buffer[0], buffer.size() );
        in.close();

        utils::Log(CFormat(L"File '%%' (size: %% bytes):\n%%")
                   << filename << buffer.size() << buffer, utils::ELogLevel::Debug);
    } else {
        throw std::runtime_error( utils::ws2s(CFormat(L"Failed to open file '%%'") << filename) );
    }

    return buffer.size();
}

size_t LoadFile(const std::string &filename, bool binary, std::string &buffer)
{
    std::ifstream in(filename,
                     binary ? std::ios::in | std::ios::binary : std::ios::in);

    if ( in.is_open() ) {
        in.seekg(0, std::ios::end);
        buffer.clear();
        buffer.resize( in.tellg() );
        in.seekg(0, std::ios::beg);

        in.read( &buffer[0], buffer.size() );
        in.close();

        utils::Log(CFormat(L"File '%%' (size: %% bytes):\n%%")
                   << filename << buffer.size() << buffer, utils::ELogLevel::Debug);
    } else {
        throw std::runtime_error( utils::ws2s(CFormat(L"Failed to open file '%%'") << filename) );
    }

    return buffer.size();
}

void CheckOpenGLError()
{
    GLenum error_code = GL_NO_ERROR;
    while ( ( error_code = glGetError() ) != GL_NO_ERROR ) {
        std::string error;

        switch (error_code) {
        case GL_INVALID_OPERATION:
            error = "GL_INVALID_OPERATION";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_INVALID_ENUM:
            error = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "GL_INVALID_VALUE";
            break;
        case GL_STACK_UNDERFLOW:
            error = "GL_STACK_UNDERFLOW";
            break;
        case GL_STACK_OVERFLOW:
            error = "GL_STACK_OVERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            error = "GL_OUT_OF_MEMORY";
            break;
        }

        utils::Log(error, utils::ELogLevel::Error);
    }
}

void APIENTRY OpenGLDebugMessageCallback(GLenum source,
                                         GLenum type,
                                         GLuint id,
                                         GLenum severity,
                                         GLsizei length,
                                         const GLchar *message,
                                         const void *user_param)
{
    UNUSED(source);
    UNUSED(length);
    UNUSED(user_param);

    CFormat debug_message(L"\n"
        L"-------------------- OpenGL Message Start --------------------\n"
        L"Message: %%\nType: %%\nID: %%\nSeverity: %%\n"
        L"--------------------  OpenGL Message End  --------------------");

    debug_message << message;

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        debug_message << "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        debug_message << "DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        debug_message << "UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        debug_message << "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        debug_message << "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        debug_message << "OTHER";
        break;
    }

    debug_message << id;

    switch (severity) {
    case GL_DEBUG_SEVERITY_LOW:
        debug_message << "LOW";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        debug_message << "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        debug_message << "HIGH";
        break;
    default:
        debug_message << "OTHER";
        break;
    }

    utils::Log(debug_message, utils::ELogLevel::Debug);
}
} // namespace utils
