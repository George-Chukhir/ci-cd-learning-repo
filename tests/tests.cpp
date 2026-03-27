#include <gtest/gtest.h>
#include "include/app.hpp"

#include <memory>


TEST(AppTest, CreateVectorReturnsCorrectSize) {

    auto app_obj = std::make_unique<App>();
    auto vec = app_obj->createVector();
    EXPECT_EQ(vec.size(), 4);
}