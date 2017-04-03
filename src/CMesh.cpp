#include <CMesh.h>

#include <CLogger.h>

#include <array>
#include <numeric>
#include <type_traits>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include <tiny_obj_loader.h>

template <typename T, std::size_t N>
constexpr T constexpr_accumulate(const std::array<T, N> &a, std::size_t i = 0u)
{
    return i < N ? (a[i] + constexpr_accumulate(a, i + 1u)) : T {};
}

constexpr std::array<unsigned, 2> SVertex::offsets;
static_assert(std::is_standard_layout<SVertex>::value, "not a standard layout");
static_assert(sizeof(SVertex) / sizeof(float) == constexpr_accumulate(SVertex::offsets), "not matching offset layout");

CMesh::CMesh()
    : _ibo(0)
{
    std::string filename = "assets/Hulk/Hulk.obj";

#if 0
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(), "assets/Hulk/");

    if (!err.empty()) { // `err` may contain warning message.
        utils::Log(utils::CFormat(L"Loading '%%': %%") << filename << err, utils::ELogLevel::Error);
    }

    if (ret)
    {
        _nodes.resize(materials.size());
        _textures.reserve(materials.size());

        // Loop over shapes
        for (size_t s = 0; s < shapes.size(); s++)
        {
            size_t index_offset = 0;
            const tinyobj::mesh_t &mesh = shapes[s].mesh;
            // Loop over faces (polygons)
            for (size_t f = 0; f < mesh.num_face_vertices.size(); f++)
            {
                size_t fv = mesh.num_face_vertices[f];

                // Per-face material id
                unsigned int material_id = static_cast<unsigned int>(mesh.material_ids[f]);
                _nodes[material_id].mesh.reserve(_nodes[material_id].mesh.size() + fv);
                _nodes[material_id].materialIndex = material_id;

                // Loop over vertices in the face
                for (size_t v = 0; v < fv; v++)
                {
                    // access to vertex
                    const tinyobj::index_t &idx = mesh.indices[index_offset + v];

                    SVertex vertex;
                    vertex.position.x = attrib.vertices[3*idx.vertex_index+0];
                    vertex.position.y = attrib.vertices[3*idx.vertex_index+1];
                    vertex.position.z = attrib.vertices[3*idx.vertex_index+2];

#if 0
                    if (idx.normal_index != -1)
                    {
                        vertex.normal.x = attrib.normals[3*idx.normal_index+0];
                        vertex.normal.y = attrib.normals[3*idx.normal_index+1];
                        vertex.normal.z = attrib.normals[3*idx.normal_index+2];
                    }
#endif

                    if (idx.texcoord_index != -1)
                    {
                        vertex.uv.x = attrib.texcoords[2*idx.texcoord_index+0];
                        vertex.uv.y = attrib.texcoords[2*idx.texcoord_index+1];
                    }

                    _nodes[material_id].mesh.emplace_back(vertex);
                }
                index_offset += fv;
            }
        }

        for (size_t m = 0; m < materials.size(); ++m) {
            _textures.emplace_back(new CTexture("assets/Hulk/" + materials[m].diffuse_texname));
        }
    }
#else
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename,
                                             aiProcess_CalcTangentSpace      |
                                             aiProcess_Triangulate           |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType);

    if (!scene)
    {
        utils::Log(utils::CFormat(L"Loading '%%': %%") << filename << importer.GetErrorString(), utils::ELogLevel::Error);
    }

    _nodes.resize(scene->mNumMeshes);
    _textures.resize(scene->mNumMaterials);

    const unsigned int texIndex = 0;

    // Loop all children
    for (unsigned int c = 0; c < scene->mRootNode->mNumChildren; ++c)
    {
        const struct aiNode* node = scene->mRootNode->mChildren[c];
        for (unsigned int m = 0; m < node->mNumMeshes; ++m)
        {
            const struct aiMesh* mesh = scene->mMeshes[node->mMeshes[m]];
            for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
            {
                const struct aiFace* face = &mesh->mFaces[f];
                for (unsigned int i = 0; i < face->mNumIndices; ++i)
                {
                    unsigned int index = face->mIndices[i];

                    SVertex vertex;
                    vertex.position.x = mesh->mVertices[index].x;
                    vertex.position.y = mesh->mVertices[index].y;
                    vertex.position.z = mesh->mVertices[index].z;

                    if (mesh->HasTextureCoords(texIndex))
                    {
                        vertex.uv.x = mesh->mTextureCoords[texIndex][index].x;
                        vertex.uv.y = mesh->mTextureCoords[texIndex][index].y;
                    }

                    _nodes[m].mesh.emplace_back(vertex);
                    _nodes[m].materialIndex = mesh->mMaterialIndex;
                }
            }
        }
    }

    // getTexture Filenames and Numb of Textures
    for (unsigned int m = 0; m < scene->mNumMaterials; ++m)
    {
        const aiMaterial* material = scene->mMaterials[m];
        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString path;
            if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS)
            {
                std::string fullPath = std::string("assets/Hulk/") + path.C_Str();
                try {
                    _textures[m].reset(new CTexture(fullPath));
                } catch (const std::exception &ex) {
                    _textures[m].reset();
                    utils::Log(ex.what(), utils::ELogLevel::Warning);
                }
            }
        }
    }
#endif

    _vao.resize(_nodes.size());
    _vbo.resize(_nodes.size());

    // Create a Vertex Array Object (VAO)
    glGenVertexArrays(_vao.size(), _vao.data());
    // Create a Vertex Buffer Object (VBO)
    glGenBuffers(_vbo.size(), _vbo.data());

#if 0
    glGenBuffers(1, &_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

    // Fill IBO with indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeElements.size() * sizeof(GLushort), cubeElements.data(), GL_STATIC_DRAW);
#endif

    for (size_t i = 0; i < _nodes.size(); ++i)
    {
        // Set VAO as the current one
        glBindVertexArray(_vao[i]);
        // Set VBO as the current one
        glBindBuffer(GL_ARRAY_BUFFER, _vbo[i]);

        // Fill VBO with vertices
        glBufferData(GL_ARRAY_BUFFER, _nodes[i].mesh.size() * sizeof(SVertex), _nodes[i].mesh.data(), GL_STATIC_DRAW);

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
    }
    glBindVertexArray(0);

    _program.AttachShader(CShader(EShaderType::Vertex, "assets/simple.vs"));
    _program.AttachShader(CShader(EShaderType::Fragment, "assets/simple.fs"));

//    CProgram::AttribInfo attribs;
//    attribs[0] = "position";
//    attribs[1] = "color";
//    _program.BindAttrib(attribs);

    _program.Link();
    _program.Use();
    _program.Validate();
}

CMesh::~CMesh()
{
#if 0
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &_ibo);
#endif

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(_vbo.size(), _vbo.data());

    glBindVertexArray(0);
    glDeleteVertexArrays(_vao.size(), _vao.data());
}

void CMesh::Render(const glm::mat4 &mvpMatrix)
{
    // Set shader program as the active one
    _program.Use();

    // Set perspective matrix in shader
    GLint mvpMatrixLocation = _program.GetUniform("mvpMatrix");
    glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

    for (size_t i = 0; i < _vao.size(); ++i)
    {
        const unsigned int materialIndex = _nodes[i].materialIndex;
        if (materialIndex < _textures.size() && _textures[materialIndex]) {
            _textures[materialIndex]->bind();
        }

        // Using VAO for rendering
        glBindVertexArray(_vao[i]);
        // Render vertices in VBO binded to VAO
        glDrawArrays(GL_TRIANGLES, 0, _nodes[i].mesh.size());

#if 0
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
        glDrawElements(GL_TRIANGLES, cubeElements.size(), GL_UNSIGNED_SHORT, 0);
#endif
    }
    glBindVertexArray(0);
}
