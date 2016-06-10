//
// Created by Matthijs Kuik on 08/06/16.
//

#include "NodeAdapter.h"
#include "Globals.h"

void NodeAdapter::notifyNodeClosed() {
    start();
    for (std::list<NodeListener *>::iterator it = listeners.begin(); it != listeners.end();) {
        activated = *it;
        activated->onNodeClosed();
        if (remove != 0) {
            it = listeners.erase(it);
        } else ++it;
    }
    stop();
}

void NodeAdapter::notifyNodeDestruct() {
    if (Globals::DESTRUCTING) return;
    start();
    for (auto it = listeners.begin(); it != listeners.end();) {
        activated = *it;
        activated->onNodeDestruct();
        if (remove == activated) {
            it = listeners.erase(it);
            remove = 0;
        } else ++it;
    }
    stop();
}

void NodeAdapter::notifyNodeMatch() {
	if(Globals::DESTRUCTING) return;
	start();
	for(auto it = listeners.begin(); it != listeners.end();) {
		activated = *it;
		activated->onNodeMatch();
		if(remove == activated) {
			it = listeners.erase(it);
			remove = 0;
		}
		else ++it;
	}
	stop();
}

