#ifndef SHADERPARSER_H
#define SHADERPARSER_H

#include <unordered_map>

#include <Common.h>

class CShaderParser
{
public:
    CShaderParser(const std::string &filename);

    const std::unordered_map<GLenum, std::string> &getShaders() const
    {
        return _shaders;
    }

private:
    std::unordered_map<GLenum, std::string> _shaders;

    static const std::unordered_map<std::string, GLenum> _shaderTypes;

    std::string parse(const std::string &path, const std::string &filename, int level);
    void addShader(GLenum type, std::stringstream &source);
};

#endif // SHADERPARSER_H
