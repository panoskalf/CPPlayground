#pragma once
#include <memory>
#include <string>
#include "TrackedInt.h"

class DummyUnique {
    friend std::ostream& operator<<(std::ostream& os, const DummyUnique& obj);
public:
    DummyUnique(const char* n, int v) : name{n}, value{std::make_unique<TrackedInt>(v)} {}
    const char* getName(void) { return name.c_str(); }
    int* getValue(void) { return &(value.get()->value); }

    static constexpr bool is_copy_deep = false;
    static constexpr bool supports_copy = false; // unique_ptr does not allow copies

private:
    std::string name;
    std::unique_ptr<TrackedInt> value;
};

std::ostream& operator<<(std::ostream& os, const DummyUnique& obj) {
    os << "(name:" << obj.name << ", ";
    os << "value: " << *(obj.value) << ")";
    return os;
}