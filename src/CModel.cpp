#include <CModel.h>

#include <numeric>
#include <type_traits>

#include <CLogger.h>

template <typename T, std::size_t N>
constexpr T constexpr_accumulate(const std::array<T, N> &a, std::size_t i = 0u)
{
    return i < N ? (a[i] + constexpr_accumulate(a, i + 1u)) : T {};
}

constexpr std::array<unsigned, SVertex::n> SVertex::offsets;
static_assert(std::is_standard_layout<SVertex>::value, "not a standard layout");
static_assert(sizeof(SVertex) / sizeof(float) == constexpr_accumulate(SVertex::offsets), "not matching offset layout");

constexpr unsigned int channel = 0;

CModel::CModel(const std::string &path)
{
    std::string basePath( utils::getBasePath(path) );

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_Fast |
                                             aiProcess_RemoveRedundantMaterials);

    if (!scene)
        throw std::runtime_error( utils::ws2s(utils::CFormat(L"Failed to load scene '%%' with error: %%")
                                              << path << importer.GetErrorString()) );

    _meshes.resize(scene->mNumMeshes);
    _materials.resize(scene->mNumMaterials);

    ProcessNode(scene, scene->mRootNode);

#ifndef NDEBUG
    for (size_t i = 0; i < _meshes.size(); ++i)
        assert(_meshes[i].vertices.size() == _meshes[i].vertices.capacity());
#endif

    // Get diffuse texture filenames
    for (unsigned int m = 0; m < scene->mNumMaterials; ++m)
    {
        const aiMaterial* material = scene->mMaterials[m];
        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString texturePath;
            if (material->GetTexture(aiTextureType_DIFFUSE, channel, &texturePath) == AI_SUCCESS)
            {
                std::string fullPath = basePath + texturePath.C_Str();
                std::replace(fullPath.begin(), fullPath.end(), '\\', '/');
                try {
                    _materials[m]._textures[TU_DIFFUSE].reset(new CTexture(fullPath));
                } catch (const std::exception &ex) {
                    _materials[m]._textures[TU_DIFFUSE].reset();
                    utils::Log(ex.what(), utils::ELogLevel::Warning);
                }
            }
        }

        if (material->GetTextureCount(aiTextureType_NORMALS) > 0 ||
            material->GetTextureCount(aiTextureType_HEIGHT) > 0)
        {
            aiString texturePath;
            if (material->GetTexture(aiTextureType_NORMALS, channel, &texturePath) == AI_SUCCESS ||
                material->GetTexture(aiTextureType_HEIGHT, channel, &texturePath) == AI_SUCCESS)
            {
                std::string fullPath = basePath + texturePath.C_Str();
                std::replace(fullPath.begin(), fullPath.end(), '\\', '/');
                try {
                    _materials[m]._textures[TU_NORMAL].reset(new CTexture(fullPath));
                } catch (const std::exception &ex) {
                    _materials[m]._textures[TU_NORMAL].reset();
                    utils::Log(ex.what(), utils::ELogLevel::Warning);
                }
            }
        }
    }

    _vao.resize(_meshes.size());
    _vbo.resize(_meshes.size());
    _ibo.resize(_meshes.size());

    for (size_t i = 0; i < _meshes.size(); ++i)
    {
        _vao[i].bind();

        _vbo[i].bind();
        _vbo[i].updateData(_meshes[i].vertices.size() * sizeof(SVertex), _meshes[i].vertices.data(), GL_STATIC_DRAW);

        for (unsigned j = 0, offset = 0; j < SVertex::offsets.size(); ++j)
        {
            _vbo[i].updateAttrib(j, SVertex::offsets[j], sizeof(SVertex), offset);
            offset += SVertex::offsets[j] * sizeof(GLfloat);
        }

        _ibo[i].bind();
        _ibo[i].updateData(_meshes[i].indices.size() * sizeof(glm::uvec3), _meshes[i].indices.data(), GL_STATIC_DRAW);
    }
    glBindVertexArray(0);
}

CModel::~CModel()
{
}

void CModel::ProcessNode(const aiScene* scene, const aiNode *node)
{
    // Loop through all meshes in a scene
    for (unsigned int m = 0; m < node->mNumMeshes; ++m) {
        ProcessMesh(scene->mMeshes[node->mMeshes[m]], _meshes[node->mMeshes[m]]);
    }

    for (unsigned int c = 0; c < node->mNumChildren; ++c) {
        ProcessNode(scene, node->mChildren[c]);
    }
}

void CModel::ProcessMesh(const aiMesh *mesh, SMesh &processedMesh)
{
#ifndef NDEBUG
    // Calc size to reserve vertex vector
    unsigned int vertexCount = 0;
    for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
        vertexCount += mesh->mFaces[f].mNumIndices;
    assert(vertexCount == mesh->mNumFaces * 3);
#endif

    processedMesh.vertices.reserve(mesh->mNumVertices);
    processedMesh.indices.reserve(mesh->mNumFaces);
    processedMesh.materialIndex = mesh->mMaterialIndex;

    for (unsigned int v = 0; v < mesh->mNumVertices; ++v)
    {
        SVertex vertex;
        vertex.position = to_vec(mesh->mVertices[v]);

        if (mesh->HasTextureCoords(channel)) {
            vertex.uv.x = mesh->mTextureCoords[channel][v].x;
            vertex.uv.y = mesh->mTextureCoords[channel][v].y;
        }

        if (mesh->HasNormals()) {
            vertex.normal = to_vec(mesh->mNormals[v]);
        }

        if (mesh->HasTangentsAndBitangents()) {
            vertex.tangent = to_vec(mesh->mTangents[v]);
            vertex.bitangent = to_vec(mesh->mBitangents[v]);
        }

        processedMesh.vertices.emplace_back(vertex);
    }

    for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
    {
        const aiFace *face = &mesh->mFaces[f];
        assert(face->mNumIndices == 3); // Faces have 3 indices, because meshes are triangulated
        processedMesh.indices.emplace_back(face->mIndices[0], face->mIndices[1], face->mIndices[2]);
    }
}

void CModel::Render()
{
    for (size_t i = 0; i < _meshes.size(); ++i)
    {
        const auto &textures = _materials[_meshes[i].materialIndex]._textures;

        for (unsigned j = 0; j < textures.size(); ++j)
        {
            if (textures[j])
            {
                glActiveTexture(GL_TEXTURE0 + j);
                textures[j]->bind();
            }
        }
        glActiveTexture(GL_TEXTURE0);

        // Using VAO for rendering
        _vao[i].bind();
        // Render vertices in VBO binded to VAO
        glDrawElements(GL_TRIANGLES, _meshes[i].indices.size() * glm::uvec3::length(), GL_UNSIGNED_INT, nullptr);
    }
    glBindVertexArray(0);
}
