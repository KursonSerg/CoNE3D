#include <CMesh.h>

#include <array>
#include <vector>
#include <numeric>
#include <type_traits>

struct SVertex
{
    glm::vec3 position;
    glm::vec2 uv;
#if 0
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
#endif

    static constexpr std::array<unsigned, 2> offsets { { 3, 2 } };
};

template <typename T, std::size_t N>
constexpr T constexpr_accumulate(const std::array<T, N> &a, std::size_t i = 0u)
{
    return i < N ? (a[i] + constexpr_accumulate(a, i + 1u)) : T {};
}

constexpr std::array<unsigned, 2> SVertex::offsets;
static_assert(std::is_standard_layout<SVertex>::value, "not a standard layout");
static_assert(sizeof(SVertex) / sizeof(float) == constexpr_accumulate(SVertex::offsets), "not matching offset layout");

static const std::vector<SVertex> cubeMesh = {
    // Back
    { {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f } },
    { {  1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f } },
    { { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f } },
    { { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f } },
    { { -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f } },
    { {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f } },

    // Front
    { { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f } },
    { {  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f } },
    { {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f } },
    { {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f } },
    { { -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f } },
    { { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f } },

    // Left
    { { -1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f } },
    { { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f } },
    { { -1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f } },
    { { -1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f } },
    { { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f } },
    { { -1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f } },

    // Right
    { {  1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f } },
    { {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f } },
    { {  1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f } },
    { {  1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f } },
    { {  1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f } },
    { {  1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f } },

    // Bottom
    { { -1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f } },
    { {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f } },
    { {  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f } },
    { {  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f } },
    { { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f } },
    { { -1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f } },

    // Top
    { {  1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f } },
    { {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f } },
    { { -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f } },
    { { -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f } },
    { { -1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f } },
    { {  1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f } }
};

CMesh::CMesh()
    : _texture("assets/crate.jpg")
    , _vao(0)
    , _vbo(0)
{
    // Create a Vertex Array Object (VAO) and set it as the current one
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // Create a Vertex Buffer Object (VBO) and set it as the current one
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    // Fill VBO with vertices
    glBufferData(GL_ARRAY_BUFFER, cubeMesh.size() * sizeof(SVertex), cubeMesh.data(), GL_STATIC_DRAW);

    const auto &offsets = SVertex::offsets;

    unsigned currentOffset = 0u;
    unsigned vertexSize = std::accumulate(offsets.begin(), offsets.end(), 0u) * sizeof(GLfloat);
    for (unsigned i = 0; i < offsets.size(); ++i)
    {
        // Set access parameters to VBO
        glVertexAttribPointer(i, offsets[i], GL_FLOAT, GL_FALSE, vertexSize, BUFFER_OFFSET(currentOffset));
        // Enable using of attribute
        glEnableVertexAttribArray(i);
        currentOffset += offsets[i] * sizeof(GLfloat);
    }

    _program.AttachShader(CShader(EShaderType::Vertex, "assets/simple.vs"));
    _program.AttachShader(CShader(EShaderType::Fragment, "assets/simple.fs"));

//    CProgram::AttribInfo attribs;
//    attribs[0] = "position";
//    attribs[1] = "color";
//    _program.BindAttrib(attribs);

    _program.Link();
    _program.Use();
    _program.Validate();

    glBindVertexArray(0);
}

CMesh::~CMesh()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &_vbo);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &_vao);
}

void CMesh::Render(const glm::mat4 &mvpMatrix)
{
    // Set shader program as the active one
    _program.Use();

    _texture.bind();

    // Set perspective matrix in shader
    GLint mvpMatrixLocation = _program.GetUniform("mvpMatrix");
    glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

    // Using VAO for rendering
    glBindVertexArray(_vao);
    // Render vertices in VBO binded to VAO
    glDrawArrays(GL_TRIANGLES, 0, cubeMesh.size());
    glBindVertexArray(0);
}
