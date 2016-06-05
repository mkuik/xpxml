//
// Created by Matthijs Kuik on 11/11/15.
//

#include "BinaryTreeAdapter.h"

void BinaryTreeAdapter::notifyAllocation(Directory* dir, Node* node) {
    start();
    for (auto it = listeners.begin(); it != listeners.end();) {
        activated = *it;
        activated->onAllocatedToBranch(dir, node);
        if (remove == activated) {
            it = listeners.erase(it);
            remove = 0;
        } else ++it;
    }
    stop();
}

void BinaryTreeAdapter::notifyRedirect(Directory* dir, Node* node) {
    start();
    for (auto it = listeners.begin(); it != listeners.end();) {
        activated = *it;
        activated->onRedirectedFromBranch(dir, node);
        if (remove == activated) {
            it = listeners.erase(it);
            remove = 0;
        } else ++it;
    }
    stop();
}