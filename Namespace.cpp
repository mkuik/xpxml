//
// Created by Matthijs on 2-7-2015.
//

#include "Namespace.h"

Namespace::Namespace(const std::string& n):
	name(n) {
	//    std::printf("Create %4i %s\n", id, name.c_str());
}

Namespace::Namespace(const Namespace& obj) {
	name = obj.name;
	//    std::printf("Copy   %4i %s\n", id, name.c_str());
}

Namespace::~Namespace() {}

const std::string& Namespace::getNamespace() const {
	return name;
}

void Namespace::setNamespace(const std::string& n) {
	name = n;
}

void Namespace::addToNamespace(const int & c) {
	name += c;
}

