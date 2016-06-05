//
// Created by Matthijs on 1-9-2015.
//

#ifndef XPXML2_FACTORY_H
#define XPXML2_FACTORY_H

#include <iostream>
#include "Node.h"
#include "Adapter.h"

class FactoryInput {
public:
    ~FactoryInput() {};
    virtual void stateChanged(Node *source) {}
    virtual void factoryDestructor(Node * source) {}
};

class FactoryOutput: public Adapter<FactoryInput> {
public:
    void notifyChange(Node *id) {
        start();
        for (std::list<FactoryInput*>::iterator it = listeners.begin(); it != listeners.end();) {
            activated = *it;
            activated->stateChanged(id);
            if (remove != 0) {
                it = listeners.erase(it);
				remove = 0;
            } else {
                ++it;
            }
        }
        stop();
        activated = 0;
    }
    void notifyDestructorUse(Node *id) {
        start();
        for (std::list<FactoryInput*>::iterator it = listeners.begin(); it != listeners.end();) {
            activated = *it;
            activated->factoryDestructor(id);
            if (remove != 0) {
                it = listeners.erase(it);
            } else {
                ++it;
            }
        }
        stop();
        activated = 0;
    }
};


#endif //XPXML2_FACTORY_H
