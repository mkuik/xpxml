/*
 * xIdentity.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: matthijs
 */

#include "Directory.h"
#include <sstream>
#include <iostream>

Directory::Directory(Directory * p, const std::string& n) :
		Name(n), Parent(p) {
}

Directory::Directory(const Directory & obj) : Name(obj), Parent(obj) {
}

Directory::~Directory() {
}

std::string Directory::getPath() const {
	std::string path = "/" + getName().substr(0, 6);
	if (hasParent())
		path = getParent()->getPath() + path;
	return path;
}

std::string Directory::toString() const {
	return getPath();
}

bool compare(Directory const * A, Directory const * B) {
	if (!A || !B)
		return true;

//	std::cout << "compare " << A->getName() << "\t" << B->getName() << std::endl;

	if (A->getName().compare(B->getName()) == 0) {
		return compare(A->getParent(), B->getParent());
	} else if (A->getName().compare("..") == 0) {
		return A->hasParent() && compare(A->getParent()->getParent(), B);
	} else return B->getName().compare("..") == 0 &&
				  B->hasParent() && compare(A, B->getParent()->getParent());
}