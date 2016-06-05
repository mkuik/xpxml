//
// Created by Matthijs Kuik on 27/10/15.
//

#include "Value.h"

Value::Value(const std::string& n) :
        value(n) {
//    std::printf("Create %4i %s\n", id, value.c_str());
}

Value::Value(const Value& obj) {
    value = obj.value;
//    std::printf("Copy   %4i %s\n", id, value.c_str());
}

Value::~Value() {
}

const std::string& Value::getValue() const {
    return value;
}

void Value::setValue(const std::string& n) {
    value = n;
}

void Value::addToValue(const int & c)
{
    value += c;
}
