#include <Common.h>
#include <CLogger.h>

#include <unordered_map>

namespace utils {
std::string getBasePath(const std::string &path)
{
    size_t pos = path.find_last_of("\\/");
    return (std::string::npos == pos) ? "" : path.substr(0, pos + 1);
}

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

static const std::unordered_map<GLenum, std::string> stringsByEnums =
{
    { GL_NO_ERROR, "no error" },
    { GL_INVALID_ENUM, "invalid enum" },
    { GL_INVALID_VALUE, "invalid value" },
    { GL_INVALID_OPERATION, "invalid operation" },
    { GL_STACK_OVERFLOW, "stack overflow" },
    { GL_STACK_UNDERFLOW, "stack underflow" },
    { GL_OUT_OF_MEMORY, "out of memory" },
    { GL_INVALID_FRAMEBUFFER_OPERATION, "invalid framebuffer operation" },
    { GL_CONTEXT_LOST, "context lost" },
    { GL_DEBUG_SOURCE_API, "API" },
    { GL_DEBUG_SOURCE_WINDOW_SYSTEM, "window system" },
    { GL_DEBUG_SOURCE_SHADER_COMPILER, "shader compiler" },
    { GL_DEBUG_SOURCE_THIRD_PARTY, "third party" },
    { GL_DEBUG_SOURCE_APPLICATION, "application" },
    { GL_DEBUG_SOURCE_OTHER, "other" },
    { GL_DEBUG_TYPE_ERROR, "error" },
    { GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "deprecated behavior" },
    { GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "undefined behavior" },
    { GL_DEBUG_TYPE_PORTABILITY, "portability" },
    { GL_DEBUG_TYPE_PERFORMANCE, "performance" },
    { GL_DEBUG_TYPE_OTHER, "other" },
    { GL_DEBUG_TYPE_MARKER, "marker" },
    { GL_DEBUG_TYPE_PUSH_GROUP, "push group" },
    { GL_DEBUG_TYPE_POP_GROUP, "pop group" },
    { GL_DEBUG_SEVERITY_HIGH, "high" },
    { GL_DEBUG_SEVERITY_MEDIUM, "medium" },
    { GL_DEBUG_SEVERITY_LOW, "low" },
    { GL_DEBUG_SEVERITY_NOTIFICATION, "notification" },
    { GL_DONT_CARE, "don't care" }
};

inline std::string stringByEnum(GLenum value)
{
    try {
        return stringsByEnums.at(value);
    } catch (const std::exception &) {
        return std::to_string(value);
    }
}

void checkErrorStatus()
{
    GLenum error = GL_NO_ERROR;
    while ( ( error = glGetError() ) != GL_NO_ERROR ) {
        utils::Log("OpenGL error: " + stringByEnum(error), utils::ELogLevel::Error);
    }
}

void APIENTRY debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                   GLsizei length, const GLchar *message, const void *user_param)
{
    UNUSED(user_param);

    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        return;

    CFormat debug_message(L"\n"
        L"-------------------- OpenGL Message Start --------------------\n"
        L"Source: %%\nType: %%\nID: %%\nSeverity: %%\nMessage: %%\n"
        L"--------------------  OpenGL Message End  --------------------");

    debug_message << stringByEnum(source) << stringByEnum(type) << id << stringByEnum(severity)
                  << std::string(message, static_cast<size_t>(length));

    utils::Log(debug_message, utils::ELogLevel::Debug);
}
} // namespace utils
