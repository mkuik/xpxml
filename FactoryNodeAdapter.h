//
// Created by Matthijs on 1-9-2015.
//

#ifndef XPXML2_FACTORY_H
#define XPXML2_FACTORY_H

#include <iostream>
#include "Node.h"
#include "Adapter.h"

class FactoryListener {
public:
    ~FactoryListener() {};
    virtual void onFactoryNodeClosedInParser(void * source, id_type id) {}
    virtual void onFactoryNodeClosedInOutput(void * source, id_type id) {}
    virtual void onFactoryNodeSourceDeleted(void * source, id_type id) {}
    virtual void onFactoryNodeDeleted(void * source, id_type id) {}
};

class FactoryAdapter: public Adapter<FactoryListener> {
public:
    void notifyFactoryNodeClosedInParser(void * source, id_type id) {
        start();
        for (std::list<FactoryListener*>::iterator it = listeners.begin(); it != listeners.end();) {
            activated = *it;
            activated->onFactoryNodeClosedInParser(source, id);
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

    void notifyFactoryNodeClosedInOutput(void * source, id_type id) {
        start();
        for (std::list<FactoryListener*>::iterator it = listeners.begin(); it != listeners.end();) {
            activated = *it;
            activated->onFactoryNodeClosedInOutput(source, id);
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

    void notifyFactoryNodeSourceDeleted(void * source, id_type id) {
        start();
        for (std::list<FactoryListener*>::iterator it = listeners.begin(); it != listeners.end();) {
            activated = *it;
            activated->onFactoryNodeSourceDeleted(source, id);
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

    void notifyFactoryNodeDeleted(void * source, id_type id) {
        start();
        for (std::list<FactoryListener*>::iterator it = listeners.begin(); it != listeners.end();) {
            activated = *it;
            activated->onFactoryNodeDeleted(source, id);
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
