#pragma once

#define GLFW_INCLUDE_NONE

#include <variant>
#include <vector>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "Camera.h"
#include "Model.h"
#include "FBXModel.h"
#include "ImGuiLayer.h"
#include <imgui.h>

class ZenithApp {
public:
    int run();

private:
    struct Preset {
        std::string name;
        std::string path;
        enum class Type { GLTF, FBX } type;
    };

    using CurrentModel = std::variant<std::monostate, Model, FBXModel>;

    void drawCurrentModel(CurrentModel& cur, Shader& shaderProgram, Camera& camera,
                          const glm::vec3& modelPos = glm::vec3(0.0f),
                          const glm::quat& modelRot = glm::quat(1.0f,0.0f,0.0f,0.0f),
                          const glm::vec3& modelScale = glm::vec3(1.0f));
};
