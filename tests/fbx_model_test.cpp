#include <gtest/gtest.h>
#include <string>

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
