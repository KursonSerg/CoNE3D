#ifndef COMMON_H
#define COMMON_H

#include <cstdint>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Logger.h>

#define UNUSED(x) ( (void)(x) )

#define BUFFER_OFFSET(x) ( reinterpret_cast<const void *>(x) )

namespace utils {
std::string getBasePath(const std::string &path);
std::string getFilename(const std::string &path);
size_t LoadFile(const std::wstring &filename, bool binary, std::wstring &buffer);
size_t LoadFile(const std::string &filename, bool binary, std::string &buffer);

void checkErrorStatus();
void APIENTRY debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                   GLsizei length, const GLchar *message, const void *user_param);
} // namespace utils

#endif // COMMON_H
