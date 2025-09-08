#include "FBXModel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <algorithm>

// Forward declarations for local helpers used below
static void normalizeSlashes(std::string& s);
static std::string filenameFromPath(const std::string& p);
static std::string toLower(std::string s);

FBXModel::FBXModel(const char* path) {
    Assimp::Importer importer;
    // Triangulate only; avoid aiProcess_FlipUVs since we already flip images via stb.
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Failed to load FBX: " << importer.GetErrorString() << std::endl;
        return;
    }
    // Compute directory robustly for Windows and POSIX paths
    std::string p = std::string(path);
    size_t posSlash = p.find_last_of('/') ;
    size_t posBack  = p.find_last_of('\\');
    size_t pos = std::string::npos;
    if (posSlash == std::string::npos) pos = posBack; else if (posBack == std::string::npos) pos = posSlash; else pos = std::max(posSlash, posBack);
    if (pos != std::string::npos) directory = p.substr(0, pos + 1); else directory = "";
    normalizeSlashes(directory);
    processNode(scene->mRootNode, scene, glm::mat4(1.0f));
}

void FBXModel::Draw(Shader& shader, Camera& camera, const glm::vec3& tr, const glm::quat& rq, const glm::vec3& sc){
    for (size_t i = 0; i < meshes.size(); ++i) {
        meshes[i].Draw(shader, camera, matrices[i], tr, rq, sc);
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

    // Determine preferred UV channel from material
    unsigned int uvChan = 0;
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        uvChan = chooseUVChannel(material);
    }
    // Fallback to first available channel if chosen is missing
    if (uvChan >= AI_MAX_NUMBER_OF_TEXTURECOORDS || !mesh->mTextureCoords[uvChan]) {
        for (unsigned int c = 0; c < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++c) {
            if (mesh->mTextureCoords[c]) { uvChan = c; break; }
        }
    }

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = mesh->HasNormals() ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) : glm::vec3(0.0f);
        if (mesh->mTextureCoords[uvChan]) {
            float u = mesh->mTextureCoords[uvChan][i].x;
            float v = mesh->mTextureCoords[uvChan][i].y;
            // Do not flip V here; STB flips the image once globally
            vertex.texUV = glm::vec2(u, v);
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
        std::vector<Texture> diffuse = loadMaterialTextures(scene, material, aiTextureType_DIFFUSE, "diffuse");
        textures.insert(textures.end(), diffuse.begin(), diffuse.end());
        if (diffuse.empty()) {
            std::cerr << "[FBX] No diffuse texture found for a mesh; will use magenta fallback.\n";
        }
    }

    return Mesh(vertices, indices, textures);
}

static void normalizeSlashes(std::string& s) {
    for (auto& c : s) if (c == '\\') c = '/';
}

static std::string filenameFromPath(const std::string& p) {
    size_t pos = p.find_last_of('/');
    if (pos == std::string::npos) return p;
    return p.substr(pos + 1);
}

#include <filesystem>

static std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return (char)std::tolower(c); });
    return s;
}

std::vector<Texture> FBXModel::loadMaterialTextures(const aiScene* scene, aiMaterial* mat, aiTextureType type, const char* typeName) {
    std::vector<Texture> textures;

    auto loadType = [&](aiTextureType t){
        unsigned int count = mat->GetTextureCount(t);
        for (unsigned int i = 0; i < count; ++i) {
            aiString str;
            if (mat->GetTexture(t, i, &str) == aiReturn_SUCCESS) {
                std::string rel = std::string(str.C_Str());
                normalizeSlashes(rel);
                // Embedded texture (e.g. "*0")
                if (!rel.empty() && rel[0] == '*') {
                    // Handle embedded textures in a future pass; skip for now
                    // to avoid false positives loading wrong files.
                    continue;
                }

                std::string texPath;
                // If rel is absolute, use it directly; otherwise join with directory
                if (!rel.empty() && (rel.size() > 1 && (rel[1] == ':' || rel[0] == '/' ))) {
                    texPath = rel;
                } else {
                    texPath = directory + rel;
                }

                // If file does not exist, try to locate by filename under the model directory
                if (!std::filesystem::exists(texPath)) {
                    std::string fname = filenameFromPath(rel);
                    for (auto const& entry : std::filesystem::recursive_directory_iterator(directory)) {
                        if (!entry.is_regular_file()) continue;
                        if (entry.path().filename().string() == fname) {
                            texPath = entry.path().string();
                            normalizeSlashes(texPath);
                            break;
                        }
                    }
                }

                std::cerr << "[FBX] " << typeName << " texture: " << texPath << (std::filesystem::exists(texPath) ? " (found)" : " (not found)") << "\n";
                textures.push_back(Texture(texPath.c_str(), typeName, i));
            }
        }
    };

    // Try PBR base color first, then classic diffuse
    loadType(aiTextureType_BASE_COLOR);
    if (textures.empty()) loadType(aiTextureType_DIFFUSE);
    // As a last resort, try 'unknown' (some exporters use it)
    if (textures.empty()) loadType(aiTextureType_UNKNOWN);

    // Heuristic: if material references no texture, try to find a likely albedo file
    if (textures.empty()) {
        static const char* kCandidates[] = { "albedo", "basecolor", "base_color", "diffuse", "color" };
        for (auto const& entry : std::filesystem::recursive_directory_iterator(directory)) {
            if (!entry.is_regular_file()) continue;
            std::string fname = toLower(entry.path().filename().string());
            for (auto patt : kCandidates) {
                if (fname.find(patt) != std::string::npos) {
                    std::string texPath = entry.path().string();
                    normalizeSlashes(texPath);
                    std::cerr << "[FBX] heuristic diffuse texture: " << texPath << " (found)\n";
                    textures.push_back(Texture(texPath.c_str(), typeName, 0));
                    break;
                }
            }
            if (!textures.empty()) break;
        }
    }

    return textures;
}

unsigned int FBXModel::chooseUVChannel(aiMaterial* mat) const {
    unsigned int uvIndex = 0;
    aiString dummy;
    aiTextureMapping mapping;
    unsigned int idx = 0;
    float blend;
    aiTextureOp op;
    aiTextureMapMode mapmode;

    // Preference order: BASE_COLOR, DIFFUSE, UNKNOWN
    if (mat->GetTextureCount(aiTextureType_BASE_COLOR) > 0 &&
        mat->GetTexture(aiTextureType_BASE_COLOR, 0, &dummy, &mapping, &uvIndex, &blend, &op, &mapmode) == aiReturn_SUCCESS) {
        return uvIndex;
    }
    if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0 &&
        mat->GetTexture(aiTextureType_DIFFUSE, 0, &dummy, &mapping, &uvIndex, &blend, &op, &mapmode) == aiReturn_SUCCESS) {
        return uvIndex;
    }
    if (mat->GetTextureCount(aiTextureType_UNKNOWN) > 0 &&
        mat->GetTexture(aiTextureType_UNKNOWN, 0, &dummy, &mapping, &uvIndex, &blend, &op, &mapmode) == aiReturn_SUCCESS) {
        return uvIndex;
    }
    return 0;
}

