#pragma once
#include <stdint.h>
#include <utility>

template<int FractionBits>
class FixedPoint {
public:
    int32_t value;

    constexpr FixedPoint() : value(0) {}
    constexpr FixedPoint(float f) : value(static_cast<int32_t>(f * (1 << FractionBits))) {}
    constexpr FixedPoint(std::pair<int32_t, int32_t> frac) : value(0) {
        *this = fromFraction(frac);
    }

    // this will not work without FPU support
    constexpr float toFloat() const {return static_cast<float>(value) / (1 << FractionBits);}
    // this will work even without FPU
    constexpr std::pair<int32_t, int32_t> toFraction() const {
        int32_t numerator = value;
        int32_t denominator = 1 << FractionBits;
        return std::make_pair(numerator, denominator);
    }

    // Operators
    constexpr FixedPoint operator+(FixedPoint other) const {return FixedPoint::fromRaw(value + other.value);}
    constexpr FixedPoint operator*(FixedPoint other) const {
        int64_t tmp = static_cast<int64_t>(value) * other.value;
        return FixedPoint::fromRaw(static_cast<int32_t>(tmp >> FractionBits));
    }

    static constexpr FixedPoint fromRaw(int32_t raw) {
        FixedPoint fp;
        fp.value = raw;
        return fp;
    }
    static constexpr FixedPoint fromFraction(std::pair<int32_t, int32_t> frac) {
        FixedPoint fp;
        fp.value = (static_cast<int64_t>(frac.first) << FractionBits) / frac.second;
        return fp;
    }
};