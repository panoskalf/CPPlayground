#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <utility>
#include <cstdint>

class DummyRaw;

class logger {
public:
    logger(const char* func_name, const DummyRaw& obj);
    ~logger();
    std::string function_name; // track function name (__func__) or custom description (string literal)
    const DummyRaw& logged_object; // track obj on entry and exit
    static int num; // numbering of logs for readability
};

class DummyRaw {
    friend std::ostream& operator<<(std::ostream& os, const DummyRaw& obj);
public:
    friend class logger;

    DummyRaw(const char* name, int value) : name{nullptr}, value{nullptr} {
        logger log("ctor", *this); // LOG

        if (name) {
            this->name = new char[strlen(name) + 1];
            strcpy(this->name, name);
        } else {
            this->name = new char[1];
            *(this->name) = '\0';
        }
        this->value = new int;
        *(this->value) = value;
    }

    ~DummyRaw() {
        logger log("dtor", *this); // LOG
        if(name) {
            delete[] name;
            name = nullptr;
        }
        if(value) {
            delete value;
            value = nullptr;
        }
    }

    // copy ctor - rule of 3
    DummyRaw(const DummyRaw& other) : name{nullptr}, value{nullptr} {
        logger log("copy ctor", *this); // LOG
        if(other.name) {
            this->name = new char[strlen(other.name) + 1];
            strcpy(this->name, other.name);
        } else {
            this->name = new char[1];
            *(this->name) = '\0';
        }

        this->value = new int;
        *(this->value) = *(other.value);
    }

    // copy assignment - rule of 3
    DummyRaw& operator=(const DummyRaw& rhs) {
        logger log("copy assignment", *this); // LOG
        if (this == &rhs) {
            return *this;
        }

        if (rhs.name) {
            const std::size_t newSize = strlen(rhs.name); // safe
            // cannot delete name without checking it was
            if (!name || strlen(name) != newSize) {
                delete[] name;
                name = new char[newSize + 1];
            }
            strcpy(name, rhs.name);
        } else {
            delete[] name;
            name = new char[1];
            *name = '\0';
        }

        // should be already allocated
        *value = *(rhs.value);
        return *this;
    }

    // move constructor - rule of 5
    DummyRaw(DummyRaw&& other)
    : name{other.name}, value{other.value} {
        logger log("move ctor", *this); // LOG
        other.name = nullptr;
        other.value = nullptr;
    }

    // move assignment - rule of 5
    DummyRaw& operator=(DummyRaw&& rhs) {
        logger log("move assignment", *this);
        if (&rhs == this) {
            return *this;
        }

        delete[] name;
        delete value;

        name = rhs.name;
        value = rhs.value;

        rhs.name = nullptr;
        rhs.value = nullptr;

        return *this;
    }

    char* getName(void) { return name; };
    int* getValue(void) { return value; };

    static constexpr bool is_copy_deep = true;
    static constexpr bool supports_copy = true;

private:
    char* name;
    int* value;
};


std::ostream& operator<<(std::ostream& os, const DummyRaw& obj) {
    // check if name ptr is null (moved object)
    if(obj.name)
    {
        os << "(name:" << obj.name << ", ";
    } else {
        os << "(name:null, ";
    }

    // check if value ptr is null (moved object)
    if(obj.value) {
        os << "value: " << *(obj.value) << ")";
    } else {
        os << "value:null)";
    }
    return os;
}

int logger::num = 0;

inline logger::logger(const char* func_name, const DummyRaw& obj)
: function_name{func_name}, logged_object{obj} {
    ++num;
    std::cout << num << ". Entering " << function_name << " at [0x" << std::hex << reinterpret_cast<uintptr_t>(&logged_object) << std::dec << "] with " << logged_object << " " << std::endl;
}

inline logger::~logger() {
    std::cout << num << ". Exiting " << function_name << " at [0x" << std::hex << reinterpret_cast<uintptr_t>(&logged_object) << std::dec << "] with " << logged_object << " " << std::endl;
}