#include "gtest/gtest.h"
#include "fixed_point.h"

using Q16 = FixedPoint<16>;

TEST(FixedPointTest, Addition)
{
    Q16 a(0.5f);
    Q16 b(0.25f);
    Q16 c = a + b;

    EXPECT_NEAR(c.toFloat(), 0.75f, 0.0001f);
}

TEST(FixedPointTest, Multiplication)
{
    Q16 a(0.5f);
    Q16 b(0.25f);
    Q16 c = a * b;

    EXPECT_NEAR(c.toFloat(), 0.125f, 0.0001f);
}