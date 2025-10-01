#pragma once
#include <iostream>

class TrackedInt {
public:
    int value;
    static int constructions;
    int instance_id;
    TrackedInt(int v) : value{v}, instance_id{++constructions} {
        std::cout << "TrackedInt[" << instance_id << "] ctor(value=" << value << ")" << std::endl;
    }
    ~TrackedInt() {
        std::cout << "TrackedInt[" << instance_id << "] dtor(value=" << value << ")" << std::endl;
    }

    // For easy printing and comparison
    operator int() const { return value; }
};

int TrackedInt::constructions = 0;