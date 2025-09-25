#include <gtest/gtest.h>
#include "add.h"

TEST(AddTest, HandlesPositiveInput) {
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(0, 0), 0);
    EXPECT_EQ(add(-1, 1), 0);
}
