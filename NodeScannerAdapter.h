//
// Created by Matthijs Kuik on 11/11/15.
//

#ifndef XPXML2_NODESCANNERADAPTER_H
#define XPXML2_NODESCANNERADAPTER_H

#include "Node.h"
#include "Adapter.h"

struct NodeScannerListener {
    virtual void onFoundNode(Node *) {};
};

struct NodeScannerAdapter : public Adapter<NodeScannerListener> {
    void notifyFoundNode(Node *node);
};

#endif //XPXML2_NODESCANNERADAPTER_H
