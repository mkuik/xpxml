//
// Created by Matthijs Kuik on 11/11/15.
//

#ifndef XPXML2_XPATHSCANNERADAPTER_H
#define XPXML2_XPATHSCANNERADAPTER_H

#include "Node.h"
#include "Adapter.h"
#include "BinaryTree.h"

struct XPathScannerListener {
    virtual void onNewXPathMatch(Node *) {};
    virtual void onNewBinaryTree(BinaryTree *) {};
    virtual void onRemoveBinaryTree(BinaryTree *) {};
};

struct XPathScannerAdapter : public Adapter<XPathScannerListener> {
    void notifyNewMatch(Node*);
    void notifyNewTree(BinaryTree*);
    void notifyRemoveTree(BinaryTree*);
};


#endif //XPXML2_XPATHSCANNERADAPTER_H
