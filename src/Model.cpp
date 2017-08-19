#include <Model.h>

#include <numeric>
#include <type_traits>

constexpr unsigned int channel = 0;

CModel::CModel(const std::string &path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_Fast |
                                             aiProcess_RemoveRedundantMaterials);

    if (!scene)
        throw std::runtime_error( utils::ws2s(utils::CFormat(L"Failed to load scene '%%' with error: %%")
                                              << path << importer.GetErrorString()) );

    utils::Log(utils::CFormat(L"Total number of\n  materials: %%\n  textures: %%\n  meshes: %%\n  lights: %%\n  cameras: %%")
               << scene->mNumMaterials << scene->mNumTextures << scene->mNumMeshes << scene->mNumLights << scene->mNumCameras, utils::ELogLevel::Info);

    _materials.resize(scene->mNumMaterials);
    _meshes.reserve(scene->mNumMeshes);

    processMaterials(scene, utils::getBasePath(path));
    processNode(scene, scene->mRootNode);
}

CModel::~CModel()
{
}

void CModel::processMaterials(const aiScene *scene, const std::string &basePath)
{
    for (unsigned int m = 0; m < scene->mNumMaterials; ++m)
    {
        _materials[m].reset(new CMaterial);
        const aiMaterial* material = scene->mMaterials[m];

        aiColor3D color;
        if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
        {
            _materials[m]->setDiffuseColor(to_vec(color));
        }
        if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
        {
            _materials[m]->setSpecularColor(to_vec(color));
        }
        if (material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
        {
            _materials[m]->setAmbientColor(to_vec(color));
        }
        float shininess;
        if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
        {
            _materials[m]->setShininess(shininess);
        }

        // Get diffuse texture filename
        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString texturePath;
            if (material->GetTexture(aiTextureType_DIFFUSE, channel, &texturePath) == AI_SUCCESS)
            {
                std::string fullPath = basePath + texturePath.C_Str();
                std::replace(fullPath.begin(), fullPath.end(), '\\', '/');
                try {
                    _materials[m]->setDiffuseTexture(new CTexture(fullPath));
                } catch (const std::exception &ex) {
                    _materials[m]->setDiffuseTexture(nullptr);
                    utils::Log(ex.what(), utils::ELogLevel::Warning);
                }
            }
        }

        // Get normal texture filename
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
                    _materials[m]->setNormalTexture(new CTexture(fullPath));
                } catch (const std::exception &ex) {
                    _materials[m]->setNormalTexture(nullptr);
                    utils::Log(ex.what(), utils::ELogLevel::Warning);
                }
            }
        }
    }
}

void CModel::processNode(const aiScene* scene, const aiNode *node)
{
    // Loop through all meshes in a scene
    for (unsigned int m = 0; m < node->mNumMeshes; ++m) {
        processMesh(scene->mMeshes[node->mMeshes[m]]);
    }

    for (unsigned int c = 0; c < node->mNumChildren; ++c) {
        processNode(scene, node->mChildren[c]);
    }
}

void CModel::processMesh(const aiMesh *mesh)
{
#ifndef NDEBUG
    // Calc size to reserve vertex vector
    unsigned int vertexCount = 0;
    for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
        vertexCount += mesh->mFaces[f].mNumIndices;
    assert(vertexCount == mesh->mNumFaces * 3);
#endif

    std::vector<CMesh::SVertex> vertices;
    std::vector<unsigned> indices;

    vertices.reserve(mesh->mNumVertices);
    indices.reserve(mesh->mNumFaces * 3);

    for (unsigned int v = 0; v < mesh->mNumVertices; ++v)
    {
        CMesh::SVertex vertex;
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

        vertices.emplace_back(vertex);
    }

    for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
    {
        const aiFace *face = &mesh->mFaces[f];
        assert(face->mNumIndices == 3); // Faces have 3 indices, because meshes are triangulated
        for (unsigned int i = 0; i < face->mNumIndices; ++i)
            indices.emplace_back(face->mIndices[i]);
    }

#ifndef NDEBUG
    assert(vertices.size() == vertices.capacity());
    assert(indices.size() == indices.capacity());
#endif

    _meshes.emplace_back(new CMesh(vertices, indices));
    _meshes.back()->setMaterial(_materials[mesh->mMaterialIndex]);
}

void CModel::render()
{
    for (size_t i = 0; i < _meshes.size(); ++i)
        _meshes[i]->render();
}
