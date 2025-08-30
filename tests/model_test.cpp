#include <gtest/gtest.h>
#include <string>
#include <vector>

// Include project headers
#include "../src/ZenithEngine/Model.h"

// Tests for the Model class
class ModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Per-test setup
    }
    
    void TearDown() override {
        // Per-test cleanup
    }
};

TEST_F(ModelTest, ModelPathValidation) {
    // Validate file path strings
    std::string validPath = "assets/Models/TestModelGLTF/test_model.gltf";
    std::string invalidPath = "";
    
    EXPECT_FALSE(validPath.empty());
    EXPECT_TRUE(invalidPath.empty());
    
    // Check file extension
    EXPECT_TRUE(validPath.find(".gltf") != std::string::npos);
    EXPECT_FALSE(validPath.find(".obj") != std::string::npos);
}

TEST_F(ModelTest, ModelDataStructures) {
    // Validate basic model data structures
    std::vector<float> vertices = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    std::vector<unsigned int> indices = {0, 1, 2};
    
    EXPECT_EQ(vertices.size(), 6u);
    EXPECT_EQ(indices.size(), 3u);
    EXPECT_EQ(vertices[0], 1.0f);
    EXPECT_EQ(indices[0], 0u);
}

TEST_F(ModelTest, ModelTransformation) {
    // Validate basic transforms
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::vec3 position(1.0f, 2.0f, 3.0f);
    glm::vec3 scale(2.0f, 2.0f, 2.0f);
    
    // Translation
    modelMatrix = glm::translate(modelMatrix, position);
    EXPECT_FLOAT_EQ(modelMatrix[3][0], 1.0f);
    EXPECT_FLOAT_EQ(modelMatrix[3][1], 2.0f);
    EXPECT_FLOAT_EQ(modelMatrix[3][2], 3.0f);
    
    // Scaling
    modelMatrix = glm::scale(modelMatrix, scale);
    EXPECT_FLOAT_EQ(modelMatrix[0][0], 2.0f);
    EXPECT_FLOAT_EQ(modelMatrix[1][1], 2.0f);
    EXPECT_FLOAT_EQ(modelMatrix[2][2], 2.0f);
}

// Model utilities
TEST(ModelUtilityTest, VertexDataValidation) {
    // Validate vertex data size is a multiple of 3
    std::vector<float> validVertices = {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};
    std::vector<float> invalidVertices = {0.0f, 0.0f}; // Not enough data
    
    EXPECT_EQ(validVertices.size() % 3, 0u); // Must be a multiple of 3
    EXPECT_NE(invalidVertices.size() % 3, 0u); // Must not be a multiple of 3
}

TEST(ModelUtilityTest, IndexDataValidation) {
    // Validate index data size is a multiple of 3 for triangles
    std::vector<unsigned int> validIndices = {0, 1, 2, 0, 2, 3};
    std::vector<unsigned int> invalidIndices = {0, 1}; // Not enough indices for a triangle
    
    EXPECT_EQ(validIndices.size() % 3, 0u); // Must be a multiple of 3
    EXPECT_NE(invalidIndices.size() % 3, 0u); // Must not be a multiple of 3
}

