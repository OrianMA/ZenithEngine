#include <gtest/gtest.h>
// Simple tests to verify Google Test works
TEST(SimpleTest, BasicAssertion) {
    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true);
    EXPECT_FALSE(false);
}

TEST(SimpleTest, MathOperations) {
    int a = 5;
    int b = 3;
    EXPECT_EQ(a + b, 8);
    EXPECT_EQ(a - b, 2);
    EXPECT_EQ(a * b, 15);
}

TEST(SimpleTest, StringOperations) {
    std::string str1 = "Hello";
    std::string str2 = "World";
    std::string result = str1 + " " + str2;
    EXPECT_EQ(result, "Hello World");
} 

