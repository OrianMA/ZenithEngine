#include "FBXModel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

FBXModel::FBXModel(const char* path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Failed to load FBX: " << importer.GetErrorString() << std::endl;
        return;
    }
    directory = std::string(path).substr(0, std::string(path).find_last_of('/') + 1);
    processNode(scene->mRootNode, scene, glm::mat4(1.0f));
}

void FBXModel::Draw(Shader& shader, Camera& camera) {
    for (size_t i = 0; i < meshes.size(); ++i) {
        meshes[i].Draw(shader, camera, matrices[i]);
    }
}

size_t FBXModel::meshCount() const {
    return meshes.size();
}

void FBXModel::processNode(aiNode* node, const aiScene* scene, glm::mat4 parentTransform) {
    aiMatrix4x4 m = node->mTransformation;
    glm::mat4 transform = parentTransform * glm::transpose(glm::make_mat4(&m.a1));

    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, transform));
        matrices.push_back(transform);
    }
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene, transform);
    }
}

Mesh FBXModel::processMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 transform) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = mesh->HasNormals() ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) : glm::vec3(0.0f);
        if (mesh->mTextureCoords[0]) {
            vertex.texUV = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            vertex.texUV = glm::vec2(0.0f);
        }
        vertex.color = glm::vec3(1.0f);
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuse = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
        textures.insert(textures.end(), diffuse.begin(), diffuse.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> FBXModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string texPath = directory + std::string(str.C_Str());
        textures.push_back(Texture(texPath.c_str(), typeName, i));
    }
    return textures;
}
