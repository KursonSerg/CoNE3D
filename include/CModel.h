#ifndef CMODEL_H
#define CMODEL_H

#include <array>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Material.h>
#include <Drawables/Mesh.h>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

class CModel
{
public:
    CModel(const std::string &path);
    ~CModel();

    void processMaterials(const aiScene *scene, const std::string &basePath);
    void processNode(const aiScene *scene, const aiNode *node);
    void processMesh(const aiMesh *mesh);

    void render();

private:
    glm::vec3 to_vec(const aiVector3D &vec) {
        return glm::vec3(vec.x, vec.y, vec.z);
    }

    std::vector<std::shared_ptr<CMaterial>> _materials;
    std::vector<std::unique_ptr<CMesh>> _meshes;
};

#endif // CMODEL_H
