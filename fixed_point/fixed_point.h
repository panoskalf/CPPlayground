#include <stdint.h>

template<int FractionBits>
class FixedPoint {
public:
    int32_t value;

    constexpr FixedPoint() : value(0) {}
    constexpr FixedPoint(float f) : value(static_cast<int32_t>(f * (1 << FractionBits))) {}

    constexpr float toFloat() const {return static_cast<float>(value) / (1 << FractionBits);}

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
};