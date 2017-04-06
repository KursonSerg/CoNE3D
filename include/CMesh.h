#ifndef CMESH_H
#define CMESH_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <CTexture.h>
#include <CShader.h>

struct SVertex
{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    static constexpr size_t n = 5;
    static constexpr std::array<unsigned, n> offsets { { 3, 2, 3, 3, 3 } };
};

struct SMesh
{
    SMesh()
        : materialIndex(0)
    {
    }

    std::vector<SVertex> mesh;
    unsigned int materialIndex;
};

class CMesh
{
public:
    CMesh(const std::string &path);
    ~CMesh();

    void Render();

private:
    std::vector<SMesh> _nodes;
    std::vector<std::unique_ptr<CTexture>> _textures;
    std::vector<std::unique_ptr<CTexture>> _normalMaps;

    std::vector<GLuint> _vao;
    std::vector<GLuint> _vbo;
    GLuint _ibo;
};

#endif // CMESH_H
