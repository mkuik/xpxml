//
// Created by Matthijs Kuik on 11/11/15.
//

#include "NodeScannerAdapter.h"

void NodeScannerAdapter::notifyFoundNode(Node *node) {
    start();
    for (std::list<NodeScannerListener *>::iterator it = listeners.begin(); it != listeners.end();) {
        activated = *it;
        activated->onFoundNode(node);
        if (remove != 0) {
            it = listeners.erase(it);
        } else ++it;
    }
    stop();
}