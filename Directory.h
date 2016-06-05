/*
 * xIdentity.h
 *
 *  Created on: Dec 9, 2014
 *      Author: matthijs
 */

#ifndef IDENTITY_H_
#define IDENTITY_H_

#include <string>
#include "Parent.h"
#include "Name.h"
#include "ObjectTypeCounter.h"

class Directory : public Name, public Parent<Directory> {
public:
    Directory(Directory *, const std::string & = "");
    Directory(const Directory &);
    virtual ~Directory();
    virtual std::string getPath() const;
    virtual std::string toString() const;
};

bool compare(Directory const *, Directory const *);

#endif /* IDENTITY_H_ */