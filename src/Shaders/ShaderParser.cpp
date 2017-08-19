#include <Shaders/ShaderParser.h>

#include <cassert>
#include <fstream>
#include <sstream>

CShaderParser::CShaderParser(const std::string &filename)
{
    parse(utils::getBasePath(filename), utils::getFilename(filename), 0);
}

std::string CShaderParser::parse(const std::string &path, const std::string &filename, int level)
{
    if (level > 32)
        throw std::runtime_error("The include depth is too high: probably cyclic includance");

    static const std::string pragma = "#pragma ";
    static const std::string include = "#include ";

    std::ifstream input(path + filename);
    if (!input.is_open())
        throw std::runtime_error( utils::ws2s(utils::CFormat(L"Failed to open %%file '%%'")
                                              << (level > 0 ? "include " : "") << path + filename) );

    std::stringstream source;

    GLenum type = 0;
    std::string line;
    while (std::getline(input, line))
    {
        std::string::size_type pos;
        if ((pos = line.find(pragma)) != std::string::npos)
        {
            addShader(type, source);
            std::string typeString = line.substr(pos + pragma.length());
            try {
                type = _shaderTypes.at(typeString);
            } catch (const std::exception &) {
                throw std::runtime_error( utils::ws2s(utils::CFormat(L"Unknown shader type '%%'") << typeString) );
            }
        }
        else if ((pos = line.find(include)) != std::string::npos)
        {
            source << parse(path, line.substr(pos + include.length()), level + 1);
        }
        else
        {
            source << line << std::endl;
        }
    }
    addShader(type, source);

    return source.str();
}

void CShaderParser::addShader(GLenum type, std::stringstream &source)
{
    if (type != 0)
    {
        _shaders.emplace(type, source.str());

        source.str(std::string());
        source.clear();
    }
}

const std::unordered_map<std::string, GLenum> CShaderParser::_shaderTypes =
{
    { "vertex", GL_VERTEX_SHADER },
    { "fragment", GL_FRAGMENT_SHADER }
};
