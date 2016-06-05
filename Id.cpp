//
// Created by Matthijs Kuik on 27/10/15.
//

#include "Id.h"

ID::ID()  {
    static id_type counter = 1;
    id = counter++;
}

ID::ID(const ID& obj) {
    id = obj.id;
}

ID::~ID() {
}

id_type ID::getID() const {
    return id;
}

