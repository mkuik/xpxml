//
// Created by Matthijs Kuik on 15-11-2015.
//

#ifndef XPXML2_SHAREDNODECONTAINER_H
#define XPXML2_SHAREDNODECONTAINER_H


#include "Node.h"

class SharedNodeContainer {
    Node* data = 0;
public:
    SharedNodeContainer();
    ~SharedNodeContainer();
    Node* getNode() const;
    virtual void setNode(Node *);

    virtual void removeNode();
};


#endif //XPXML2_SHAREDNODECONTAINER_H
