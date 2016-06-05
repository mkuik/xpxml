//
// Created by Matthijs on 2-7-2015.
//

#include "Name.h"
#include <typeinfo>

Name::Name(const std::string& n) :
        name(n) {
}

Name::Name(const Name& obj) {
    name = obj.name;
}

Name::~Name() {
}

const std::string& Name::getName() const {
    return name;
}

void Name::setName(const std::string& n) {
    name = n;
}

void Name::addToName(const int & c)
{
	name += c;
}

