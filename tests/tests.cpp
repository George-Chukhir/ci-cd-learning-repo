#include <gtest/gtest.h>
#include "src/app.cpp"


TEST(AppTest, MainFunctionDoesNotThrow) {
    EXPECT_NO_THROW(main());
}

TEST(AppTest, CreateVectorReturnsCorrectSize) {
    auto vec = createVector();
    EXPECT_EQ(vec.size(), 4);
}