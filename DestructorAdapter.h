//
// Created by Matthijs Kuik on 25-2-2016.
//

#ifndef XPXML2_DESTRUCTORADAPTER_H
#define XPXML2_DESTRUCTORADAPTER_H

#include "Adapter.h"

struct DestructorListener {
    virtual void onDestruct(void *) {};
};

struct DestructorAdapter : public Adapter<DestructorListener>{
    virtual void notifyDestruct(void *);
};


#endif //XPXML2_DESTRUCTORADAPTER_H
