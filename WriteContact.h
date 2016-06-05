//
// Created by Matthijs Kuik on 12/10/15.
//

#ifndef XPXML2_OUTPUTCONTACT_H
#define XPXML2_OUTPUTCONTACT_H

#include <iostream>
#include "Adapter.h"

class WriteInput {
public:
    ~WriteInput() {};
    virtual void writeResult(std::stringstream& stream) {}
    virtual void newHit(Node * node) {}
};

class WriteOutput: public Adapter<WriteInput> {
public:
    void notifyOutput(std::stringstream& stream) {
        start();
        for (std::list<WriteInput*>::iterator it = listeners.begin(); it != listeners.end();) {
            activated = *it;
            activated->writeResult(stream);

            if (remove != 0) {
                it = listeners.erase(it);
            } else {
                ++it;
            }
        }
        stop();
        activated = 0;
    }
    void notifyHit(Node * node) {
        start();
        for (std::list<WriteInput*>::iterator it = listeners.begin(); it != listeners.end();) {
            activated = *it;
            activated->newHit(node);

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

#endif //XPXML2_OUTPUTCONTACT_H
