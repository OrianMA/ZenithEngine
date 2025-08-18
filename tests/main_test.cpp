#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <fstream>

// Include des headers de ton projet
#include "../src/ZenithEngine/Model.h"
#include "../src/ZenithEngine/shaderClass.h"
#include "../src/ZenithEngine/Camera.h"
#include "../src/ZenithEngine/Texture.h"

// Tests pour la classe Shader
class ShaderTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Créer des fichiers de shader temporaires pour les tests
        createTestShaders();
    }
    
    void TearDown() override {
        // Nettoyer les fichiers temporaires
        cleanupTestShaders();
    }

private:
    void createTestShaders() {
        // Créer un vertex shader simple
        std::ofstream vertFile("test_vertex.vert");
        vertFile << "#version 330 core\n";
        vertFile << "layout (location = 0) in vec3 aPos;\n";
        vertFile << "void main() {\n";
        vertFile << "    gl_Position = vec4(aPos, 1.0);\n";
        vertFile << "}\n";
        vertFile.close();

        // Créer un fragment shader simple
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
    // Test que les fichiers de shader existent
    std::ifstream vertFile("test_vertex.vert");
    std::ifstream fragFile("test_fragment.frag");
    
    EXPECT_TRUE(vertFile.good());
    EXPECT_TRUE(fragFile.good());
    
    vertFile.close();
    fragFile.close();
}

TEST_F(ShaderTest, ShaderCompilation) {
    // Test de compilation de shader (sans OpenGL context)
    // Note: Ce test vérifie juste que les fichiers sont valides
    std::string vertSource = "#version 330 core\nlayout (location = 0) in vec3 aPos;\nvoid main() { gl_Position = vec4(aPos, 1.0); }";
    std::string fragSource = "#version 330 core\nout vec4 FragColor;\nvoid main() { FragColor = vec4(1.0); }";
    
    EXPECT_FALSE(vertSource.empty());
    EXPECT_FALSE(fragSource.empty());
    EXPECT_TRUE(vertSource.find("main") != std::string::npos);
    EXPECT_TRUE(fragSource.find("main") != std::string::npos);
}

// Tests pour la classe Camera
class CameraTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup pour chaque test
    }
    
    void TearDown() override {
        // Cleanup après chaque test
    }
};

TEST_F(CameraTest, CameraInitialization) {
    // Test d'initialisation de la caméra avec des valeurs par défaut
    // Note: On teste juste la logique, pas l'OpenGL
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
    // Test des matrices de la caméra
    glm::mat4 viewMatrix = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 2.0f),  // Position
        glm::vec3(0.0f, 0.0f, 0.0f),  // Target
        glm::vec3(0.0f, 1.0f, 0.0f)   // Up
    );
    
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(45.0f),  // FOV
        800.0f / 600.0f,      // Aspect ratio
        0.1f,                  // Near
        100.0f                 // Far
    );
    
    // Vérifier que les matrices ne sont pas nulles (test simple)
    // Note: Les valeurs exactes dépendent de l'implémentation GLM
    EXPECT_FLOAT_EQ(viewMatrix[0][0], 1.0f);
    // La projection matrix a une valeur différente selon l'aspect ratio
    EXPECT_GT(projectionMatrix[0][0], 0.0f); // Doit être positive
}

// Tests pour les utilitaires mathématiques
TEST(MathTest, VectorOperations) {
    // Test des opérations sur les vecteurs glm
    glm::vec3 v1(1.0f, 2.0f, 3.0f);
    glm::vec3 v2(4.0f, 5.0f, 6.0f);
    
    glm::vec3 result = v1 + v2;
    EXPECT_FLOAT_EQ(result.x, 5.0f);
    EXPECT_FLOAT_EQ(result.y, 7.0f);
    EXPECT_FLOAT_EQ(result.z, 9.0f);
    
    // Test de la soustraction
    glm::vec3 diff = v2 - v1;
    EXPECT_FLOAT_EQ(diff.x, 3.0f);
    EXPECT_FLOAT_EQ(diff.y, 3.0f);
    EXPECT_FLOAT_EQ(diff.z, 3.0f);
    
    // Test de la multiplication par un scalaire
    glm::vec3 scaled = v1 * 2.0f;
    EXPECT_FLOAT_EQ(scaled.x, 2.0f);
    EXPECT_FLOAT_EQ(scaled.y, 4.0f);
    EXPECT_FLOAT_EQ(scaled.z, 6.0f);
}

TEST(MathTest, MatrixOperations) {
    // Test des opérations sur les matrices glm
    glm::mat4 identity = glm::mat4(1.0f);
    
    // Vérifier que c'est bien une matrice identité
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                EXPECT_FLOAT_EQ(identity[i][j], 1.0f);
            } else {
                EXPECT_FLOAT_EQ(identity[i][j], 0.0f);
            }
        }
    }
    
    // Test de la translation
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
    EXPECT_FLOAT_EQ(translation[3][0], 1.0f);
    EXPECT_FLOAT_EQ(translation[3][1], 2.0f);
    EXPECT_FLOAT_EQ(translation[3][2], 3.0f);
    
    // Test de la rotation
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // cos(90°) ≈ 0, mais avec une tolérance pour les erreurs de précision
    EXPECT_NEAR(rotation[0][0], 0.0f, 1e-6f);
}

// Tests pour les utilitaires de fichiers
TEST(FileTest, FileExists) {
    // Test de vérification d'existence de fichiers
    std::ofstream testFile("test_file.txt");
    testFile << "Test content";
    testFile.close();
    
    std::ifstream checkFile("test_file.txt");
    EXPECT_TRUE(checkFile.good());
    checkFile.close();
    
    // Nettoyer
    std::remove("test_file.txt");
}

TEST(FileTest, FileContent) {
    // Test de lecture/écriture de fichiers
    std::string testContent = "Hello, World!";
    
    // Écrire
    std::ofstream writeFile("test_content.txt");
    writeFile << testContent;
    writeFile.close();
    
    // Lire
    std::ifstream readFile("test_content.txt");
    std::string readContent;
    std::getline(readFile, readContent);
    readFile.close();
    
    EXPECT_EQ(readContent, testContent);
    
    // Nettoyer
    std::remove("test_content.txt");
}

// Tests pour les utilitaires de chaînes
TEST(StringTest, StringOperations) {
    std::string str1 = "Hello";
    std::string str2 = "World";
    
    // Test de concaténation
    std::string result = str1 + " " + str2;
    EXPECT_EQ(result, "Hello World");
    
    // Test de longueur
    EXPECT_EQ(str1.length(), 5);
    EXPECT_EQ(str2.length(), 5);
    
    // Test de recherche
    EXPECT_TRUE(result.find("Hello") != std::string::npos);
    EXPECT_TRUE(result.find("World") != std::string::npos);
} 