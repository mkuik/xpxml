//
// Created by Matthijs Kuik on 08/06/16.
//

#ifndef XPXML2_NODEADAPTER_H
#define XPXML2_NODEADAPTER_H


#include "Adapter.h"

struct NodeListener {
    virtual void onNodeClosed() {};
    virtual void onNodeDestruct() {};
};

struct NodeAdapter : public Adapter<NodeListener> {
    void notifyNodeClosed();
    void notifyNodeDestruct();
};


#endif //XPXML2_NODEADAPTER_H
