#ifndef SHADER_H
#define SHADER_H

#include <unordered_map>

#include <Common.h>

class CShader
{
public:
    CShader(GLenum type, const std::string &source);
    ~CShader();

    GLuint getId() const
    {
        return _id;
    }

private:
    GLuint _id;

    void compile(const std::string &source);
    void check(GLenum status);
};

#endif // SHADER_H
