#ifndef FBXMODEL_CLASS_H
#define FBXMODEL_CLASS_H

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class FBXModel {
public:
    // Loads a model from an FBX file using Assimp
    explicit FBXModel(const char* path);

    // Draw all meshes of the model
    void Draw(Shader& shader, Camera& camera, const glm::vec3& tr = glm::vec3(0.0f), const glm::quat& rq = glm::quat(1.0f,0.0f,0.0f,0.0f), const glm::vec3& sc = glm::vec3(1.0f));

    // Returns the number of meshes loaded in this model
    size_t meshCount() const;

private:
    std::vector<Mesh> meshes;
    std::vector<glm::mat4> matrices;
    std::string directory;

    void processNode(aiNode* node, const aiScene* scene, glm::mat4 parentTransform);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 transform);
    std::vector<Texture> loadMaterialTextures(const aiScene* scene, aiMaterial* mat, aiTextureType type, const char* typeName);
    unsigned int chooseUVChannel(aiMaterial* mat) const;
};

#endif

