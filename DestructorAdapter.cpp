//
// Created by Matthijs Kuik on 25-2-2016.
//

#include "DestructorAdapter.h"
#include "Globals.h"

void DestructorAdapter::notifyDestruct(void * pointer) {
    if (Globals::DESTRUCTING) return;
    start();
    for (auto it = listeners.begin(); it != listeners.end();) {
        activated = *it;
        activated->onDestruct(pointer);
        if (remove == activated) {
            it = listeners.erase(it);
            remove = 0;
        } else ++it;
    }
    stop();
}
