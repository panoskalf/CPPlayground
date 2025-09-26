#include "gtest/gtest.h"
#include "fixed_point.h"
#include <vector>

using Q16 = FixedPoint<16>;

// 1. Simple tests
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

TEST(FixedPointTest, Fraction)
{
    Q16 a(3.5f);
    std::pair<int32_t, int32_t> fraction = a.toFraction();

    EXPECT_NEAR(static_cast<float>(fraction.first) / fraction.second, 3.5f, 0.0001f);
}

// 2. Parameterized tests - Test multiple fractions at once
class FractionTest : public ::testing::TestWithParam<std::tuple<int32_t, int32_t, float>> {
protected:
    void SetUp() override {
        // optional setup code
    }
};

TEST_P(FractionTest, FromFractionRoundTrip) {
    auto [numerator, denominator, expected_float] = GetParam();

    Q16 fp = Q16::fromFraction({numerator, denominator});
    auto [num_back, den_back] = fp.toFraction();

    EXPECT_NEAR(static_cast<float>(num_back) / den_back, expected_float, 0.0001f);

    Q16 reconstructed({num_back, den_back});
    EXPECT_EQ(fp.value, reconstructed.value);
}

INSTANTIATE_TEST_SUITE_P(
    CommonFractions,
    FractionTest,
    ::testing::Values(
        std::make_tuple(1, 2, 0.5f),
        std::make_tuple(3, 4, 0.75f),
        std::make_tuple(22, 7, 3.14285f),
        std::make_tuple(1, 3, 0.33333f),
        std::make_tuple(-5, 8, -0.625f),
        std::make_tuple(6, 2, 3.0f)
    )
);

// 3. Fixture class - Share setup between related tests
class FixedPointFractionFixture : public ::testing::Test {
protected:
    void SetUp() override {
        half = Q16::fromFraction({1,2});
        quarter = Q16::fromFraction({1,4});
        three_quarters = Q16::fromFraction({3,4});
        pi_approx = Q16::fromFraction({22,7});
    }

    Q16 half, quarter, three_quarters, pi_approx;
};

TEST_F(FixedPointFractionFixture, FractionArithmetic) {
    Q16 result = half + quarter;
    EXPECT_EQ(result.value, three_quarters.value);

    Q16 result2 = half * half;
    EXPECT_EQ(result2.value, quarter.value);
}

TEST_F(FixedPointFractionFixture, FractionComparison) {
    EXPECT_GT(three_quarters.value, half.value);
    EXPECT_LT(quarter.value, half.value);
}

// 4. Helper function - More readable assertions
bool IsApproximatelyEqual(float actual, float expected, float tolerance) {
    return std::abs(actual - expected) <= tolerance;
}

TEST(FixedPointTest, HelperFunctionExample) {
    Q16 fp = Q16::fromFraction({355, 113}); // better pi approximation
    EXPECT_TRUE(IsApproximatelyEqual(fp.toFloat(), 3.14159f, 0.0001f));
}

// 5. Death tests - test error conditions
TEST(FixedPointTest, DivisionByZero) {
    EXPECT_DEATH(Q16::fromFraction({1, 0}), ".*"); // Dies with any message
}

// 6. Value parameterized tests with custom names
struct FractionTestData {
    int32_t num, den;
    const char* name;
    float expected;
};

// make the ouput clearer
std::ostream& operator<<(std::ostream& os, const FractionTestData& data) {
    return os << data.name << "(" << data.num << "/" << data.den << "=" << data.expected << ")";
}

class NamedFractionTest : public ::testing::TestWithParam<FractionTestData> {};

TEST_P(NamedFractionTest, AccuracyTest) {
    const auto& data = GetParam();
    Q16 fp = Q16::fromFraction({data.num, data.den});
    EXPECT_NEAR(fp.toFloat(), data.expected, 0.001f) << "Failed for fraction " << data.name;
}

INSTANTIATE_TEST_SUITE_P(
    FractionAccuracy,
    NamedFractionTest,
    ::testing::Values(
        FractionTestData{1, 2, "half", 0.5},
        FractionTestData{22, 7, "pi_rough", 3.14286f},
        FractionTestData{355, 113, "pi_precise", 3.14159f}
    ),
    [](const ::testing::TestParamInfo<FractionTestData>& info) {
        return info.param.name; // Use custom names for test cases
    }
);

// 7. Typed tests - Test different FractionBits value
template <typename T>
class TypedFractionTest : public ::testing::Test {};

using FixedPointTypes = ::testing::Types<FixedPoint<8>, FixedPoint<16>, FixedPoint<24>>;
TYPED_TEST_SUITE(TypedFractionTest, FixedPointTypes);

TYPED_TEST(TypedFractionTest, BasicFraction) {
    TypeParam fp = TypeParam::fromFraction({1, 2});
    EXPECT_NEAR(fp.toFloat(), 0.5f, 0.01f);
}