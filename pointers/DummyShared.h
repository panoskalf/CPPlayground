#pragma once
#include <string>
#include <memory>
#include "TrackedInt.h"

class DummyShared {
    friend std::ostream& operator<<(std::ostream& os, const DummyShared& obj);
public:
    DummyShared(const char* n, int v) :
    name{n}, value{std::make_shared<TrackedInt>(v)} {}

    const char* getName(void) { return name.c_str(); }
    int* getValue(void) { return &(value.get()->value); }
    std::shared_ptr<TrackedInt>& getSharedPtr() { return value; }

    static constexpr bool is_copy_deep = false; // shared_ptr will shallow copy and increase counter
    static constexpr bool supports_copy = true;

private:
    std::string name;
    std::shared_ptr<TrackedInt> value;
};


std::ostream& operator<<(std::ostream& os, const DummyShared& obj) {
    os << "(name:" << obj.name << ", ";
    os << "value: " << *obj.value << ")";
    return os;
}