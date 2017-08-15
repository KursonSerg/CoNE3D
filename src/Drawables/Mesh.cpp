#include <Drawables/Mesh.h>

CMesh::CMesh(const std::vector<SVertex> &vertices, const std::vector<unsigned> &indices)
    : CIndexedDrawable(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT)
{
    updateVertices(vertices.size() * sizeof(SVertex), vertices.data(), GL_STATIC_DRAW);
    for (unsigned j = 0, offset = 0; j < SVertex::offsets.size(); ++j)
    {
        updateAttrib(j, SVertex::offsets[j], SVertex::size, offset);
        offset += SVertex::offsets[j] * sizeof(GLfloat);
    }
    updateIndices(indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);
}

void CMesh::render() const
{
    if (_material)
        _material->bind();
    CIndexedDrawable::render();
}

constexpr std::array<unsigned, CMesh::SVertex::n> CMesh::SVertex::offsets;
