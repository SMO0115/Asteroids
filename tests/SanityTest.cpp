//
// Created by marcel on 12/24/25.
//

#include <gtest/gtest.h>

// A simple test to check if 1+1 is 2
TEST(SanityCheck, BasicMath) {
    EXPECT_EQ(1 + 1, 2);
}

// A test to ensure GTest is linked correctly
TEST(SanityCheck, Strings) {
    std::string s = "Engine";
    EXPECT_EQ(s.length(), 6);
}