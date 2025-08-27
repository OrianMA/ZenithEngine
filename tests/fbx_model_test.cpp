#include <gtest/gtest.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <assimp/material.h>

#include "../src/ZenithEngine/FBXModel.h"

class FBXModelTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(FBXModelTest, PathHandling) {
    std::string path = "assets/Models/test_model.fbx";
    EXPECT_TRUE(path.find(".fbx") != std::string::npos);
}

TEST_F(FBXModelTest, MeshCountInitiallyZero) {
    FBXModel model("non_existing.fbx");
    EXPECT_EQ(model.meshCount(), 0u);
}

TEST_F(FBXModelTest, UVTransformAppliesScalingAndTranslation) {
    aiUVTransform t;
    t.mScaling = aiVector2D(2.0f, 0.5f);
    t.mTranslation = aiVector2D(0.1f, -0.2f);
    t.mRotation = 0.0f;
    glm::vec2 uv(0.25f, 0.4f);
    glm::vec2 result = FBXModel::applyUVTransform(uv, t);
    EXPECT_FLOAT_EQ(result.x, uv.x * 2.0f + 0.1f);
    EXPECT_FLOAT_EQ(result.y, uv.y * 0.5f - 0.2f);
}

TEST_F(FBXModelTest, UVTransformAppliesRotation) {
    aiUVTransform t;
    t.mScaling = aiVector2D(1.0f, 1.0f);
    t.mTranslation = aiVector2D(0.0f, 0.0f);
    t.mRotation = glm::half_pi<float>();
    glm::vec2 uv(1.0f, 0.0f);
    glm::vec2 result = FBXModel::applyUVTransform(uv, t);
    EXPECT_NEAR(result.x, 0.0f, 1e-5);
    EXPECT_NEAR(result.y, 1.0f, 1e-5);
}
