#ifndef MESH_H
#define MESH_H

#include <array>
#include <vector>

#include <glm/glm.hpp>

#include <Drawables/IndexedDrawable.h>
#include <Material.h>

template <typename T, std::size_t N>
constexpr T accumulate(const std::array<T, N> &a, std::size_t i = 0u)
{
    return i < N ? (a[i] + accumulate(a, i + 1u)) : T {};
}

class CMesh : private CIndexedDrawable
{
public:
    struct SVertex
    {
        glm::vec3 position;
        glm::vec2 uv;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 bitangent;

        static constexpr size_t n = 5;
        static constexpr std::array<unsigned, n> offsets { { 3, 2, 3, 3, 3 } };
        static constexpr size_t size = accumulate(offsets) * sizeof(float);
    };
    static_assert(std::is_standard_layout<SVertex>::value, "not a standard layout");
    static_assert(sizeof(SVertex) == SVertex::size, "not matching offset layout");

public:
    CMesh(const std::vector<SVertex> &vertices, const std::vector<unsigned> &indices);

    const std::shared_ptr<CMaterial> &getMaterial() { return _material; }
    void setMaterial(const std::shared_ptr<CMaterial> &material) { _material = material; }

    virtual void render() const override;

private:
    std::shared_ptr<CMaterial> _material;
};

#endif // MESH_H
