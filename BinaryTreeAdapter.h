//
// Created by Matthijs Kuik on 11/11/15.
//

#ifndef XPXML2_BINARYTREEADAPTER_H
#define XPXML2_BINARYTREEADAPTER_H

#include "Node.h"
#include "Adapter.h"

struct BinaryTreeListener {
    virtual void onAllocatedToBranch(Directory*, Node*) {};
    virtual void onRedirectedFromBranch(Directory*, Node*) {};
};

struct BinaryTreeAdapter : public Adapter<BinaryTreeListener> {
    void notifyAllocation(Directory*, Node*);
    void notifyRedirect(Directory*, Node*);
};

#endif //XPXML2_BINARYTREEADAPTER_H
