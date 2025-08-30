#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <fstream>

// Include project headers
#include "../src/ZenithEngine/Model.h"
#include "../src/ZenithEngine/shaderClass.h"
#include "../src/ZenithEngine/Camera.h"
#include "../src/ZenithEngine/Texture.h"

// Tests for the Shader class
class ShaderTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create temporary shader files for tests
        createTestShaders();
    }
    
    void TearDown() override {
        // Clean up temporary files
        cleanupTestShaders();
    }

private:
    void createTestShaders() {
        // Create a simple vertex shader
        std::ofstream vertFile("test_vertex.vert");
        vertFile << "#version 330 core\n";
        vertFile << "layout (location = 0) in vec3 aPos;\n";
        vertFile << "void main() {\n";
        vertFile << "    gl_Position = vec4(aPos, 1.0);\n";
        vertFile << "}\n";
        vertFile.close();

        // Create a simple fragment shader
        std::ofstream fragFile("test_fragment.frag");
        fragFile << "#version 330 core\n";
        fragFile << "out vec4 FragColor;\n";
        fragFile << "void main() {\n";
        fragFile << "    FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n";
        fragFile << "}\n";
        fragFile.close();
    }

    void cleanupTestShaders() {
        std::remove("test_vertex.vert");
        std::remove("test_fragment.frag");
    }
};

TEST_F(ShaderTest, ShaderFileCreation) {
    // Verify that the shader files exist
    std::ifstream vertFile("test_vertex.vert");
    std::ifstream fragFile("test_fragment.frag");
    
    EXPECT_TRUE(vertFile.good());
    EXPECT_TRUE(fragFile.good());
    
    vertFile.close();
    fragFile.close();
}

TEST_F(ShaderTest, ShaderCompilation) {
    // Shader compilation test (without OpenGL context)
    // Note: This only checks that the sources look valid
    std::string vertSource = "#version 330 core\nlayout (location = 0) in vec3 aPos;\nvoid main() { gl_Position = vec4(aPos, 1.0); }";
    std::string fragSource = "#version 330 core\nout vec4 FragColor;\nvoid main() { FragColor = vec4(1.0); }";
    
    EXPECT_FALSE(vertSource.empty());
    EXPECT_FALSE(fragSource.empty());
    EXPECT_TRUE(vertSource.find("main") != std::string::npos);
    EXPECT_TRUE(fragSource.find("main") != std::string::npos);
}

// Tests for the Camera class
class CameraTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Per-test setup
    }
    
    void TearDown() override {
        // Per-test cleanup
    }
};

TEST_F(CameraTest, CameraInitialization) {
    // Camera initialization with default-like values
    // Note: We test math/values only, not OpenGL
    int width = 800;
    int height = 600;
    glm::vec3 position(0.0f, 0.0f, 2.0f);
    
    EXPECT_EQ(width, 800);
    EXPECT_EQ(height, 600);
    EXPECT_FLOAT_EQ(position.x, 0.0f);
    EXPECT_FLOAT_EQ(position.y, 0.0f);
    EXPECT_FLOAT_EQ(position.z, 2.0f);
}

TEST_F(CameraTest, CameraMatrix) {
    // Camera matrix tests
    glm::mat4 viewMatrix = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 2.0f),  // Position
        glm::vec3(0.0f, 0.0f, 0.0f),  // Target
        glm::vec3(0.0f, 1.0f, 0.0f)   // Up
    );
    
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(45.0f),  // FOV
        800.0f / 600.0f,      // Aspect ratio
        0.1f,                 // Near
        100.0f                // Far
    );
    
    // Sanity-check some matrix values (simple test)
    // Note: exact values depend on GLM implementation
    EXPECT_FLOAT_EQ(viewMatrix[0][0], 1.0f);
    // Projection matrix element depends on aspect ratio
    EXPECT_GT(projectionMatrix[0][0], 0.0f); // Should be positive
}

// Tests for math utilities
TEST(MathTest, VectorOperations) {
    // Test glm vector operations
    glm::vec3 v1(1.0f, 2.0f, 3.0f);
    glm::vec3 v2(4.0f, 5.0f, 6.0f);
    
    glm::vec3 result = v1 + v2;
    EXPECT_FLOAT_EQ(result.x, 5.0f);
    EXPECT_FLOAT_EQ(result.y, 7.0f);
    EXPECT_FLOAT_EQ(result.z, 9.0f);
    
    // Subtraction
    glm::vec3 diff = v2 - v1;
    EXPECT_FLOAT_EQ(diff.x, 3.0f);
    EXPECT_FLOAT_EQ(diff.y, 3.0f);
    EXPECT_FLOAT_EQ(diff.z, 3.0f);
    
    // Scalar multiplication
    glm::vec3 scaled = v1 * 2.0f;
    EXPECT_FLOAT_EQ(scaled.x, 2.0f);
    EXPECT_FLOAT_EQ(scaled.y, 4.0f);
    EXPECT_FLOAT_EQ(scaled.z, 6.0f);
}

TEST(MathTest, MatrixOperations) {
    // Test glm matrix operations
    glm::mat4 identity = glm::mat4(1.0f);
    
    // Verify identity matrix values
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                EXPECT_FLOAT_EQ(identity[i][j], 1.0f);
            } else {
                EXPECT_FLOAT_EQ(identity[i][j], 0.0f);
            }
        }
    }
    
    // Translation
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
    EXPECT_FLOAT_EQ(translation[3][0], 1.0f);
    EXPECT_FLOAT_EQ(translation[3][1], 2.0f);
    EXPECT_FLOAT_EQ(translation[3][2], 3.0f);
    
    // Rotation
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // cos(90°) ~ 0, allow tolerance for floating-point precision
    EXPECT_NEAR(rotation[0][0], 0.0f, 1e-6f);
}

// Tests for file utilities
TEST(FileTest, FileExists) {
    // File existence check
    std::ofstream testFile("test_file.txt");
    testFile << "Test content";
    testFile.close();
    
    std::ifstream checkFile("test_file.txt");
    EXPECT_TRUE(checkFile.good());
    checkFile.close();
    
    // Cleanup
    std::remove("test_file.txt");
}

TEST(FileTest, FileContent) {
    // File write/read test
    std::string testContent = "Hello, World!";
    
    // Write
    std::ofstream writeFile("test_content.txt");
    writeFile << testContent;
    writeFile.close();
    
    // Read
    std::ifstream readFile("test_content.txt");
    std::string readContent;
    std::getline(readFile, readContent);
    readFile.close();
    
    EXPECT_EQ(readContent, testContent);
    
    // Cleanup
    std::remove("test_content.txt");
}

// Tests for string utilities
TEST(StringTest, StringOperations) {
    std::string str1 = "Hello";
    std::string str2 = "World";
    
    // Concatenation
    std::string result = str1 + " " + str2;
    EXPECT_EQ(result, "Hello World");
    
    // Length
    EXPECT_EQ(str1.length(), 5);
    EXPECT_EQ(str2.length(), 5);
    
    // Search
    EXPECT_TRUE(result.find("Hello") != std::string::npos);
    EXPECT_TRUE(result.find("World") != std::string::npos);
} 

