//
// Created by Matthijs Kuik on 6-10-2015.
//

#include "ObjectTypeCounter.h"

ObjectTypeCounter::ObjectTypeCounter() {
    ++count;
}

ObjectTypeCounter::~ObjectTypeCounter() {
    --count;
}

unsigned int ObjectTypeCounter::count = 0;

unsigned int ObjectTypeCounter::getTypeCount() const {
    return count;
}
