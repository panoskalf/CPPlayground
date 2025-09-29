#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <utility>
#include <cstdint>

class demo_object;

class logger {
public:
    logger(const char* func_name, const demo_object& obj);
    ~logger();
    std::string function_name; // track function name (__func__) or custom description (string literal)
    const demo_object& logged_object; // track obj on entry and exit
    static int num; // numbering of logs for readability
};

class demo_object {
    friend std::ostream& operator<<(std::ostream& os, const demo_object& obj);
public:
    friend class logger;

    demo_object(const char* name, int value) : name{nullptr}, value{nullptr} {
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

    ~demo_object() {
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
    demo_object(const demo_object& other) : name{nullptr}, value{nullptr} {
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
    demo_object& operator=(const demo_object& rhs) {
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

        // TODO: check for num rhs.value or this->value?
        *value = *(rhs.value);
        return *this;
    }

    // move constructor - rule of 5
    demo_object(demo_object&& other)
    : name{other.name}, value{other.value} {
        logger log("move ctor", *this); // LOG
        other.name = nullptr;
        other.value = nullptr;
    }

    // move assignment - rule of 5
    demo_object& operator=(demo_object&& rhs) {
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

private:
    char* name;
    int* value;
};


std::ostream& operator<<(std::ostream& os, const demo_object& obj) {
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

inline logger::logger(const char* func_name, const demo_object& obj)
: function_name{func_name}, logged_object{obj} {
    ++num;
    std::cout << num << ". Entering " << function_name << " at [0x" << std::hex << reinterpret_cast<uintptr_t>(&logged_object) << std::dec << "] with " << logged_object << " " << std::endl;
}

inline logger::~logger() {
    std::cout << num << ". Exiting " << function_name << " at [0x" << std::hex << reinterpret_cast<uintptr_t>(&logged_object) << std::dec << "] with " << logged_object << " " << std::endl;
}