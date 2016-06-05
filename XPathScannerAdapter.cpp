//
// Created by Matthijs Kuik on 11/11/15.
//

#include "XPathScannerAdapter.h"

void XPathScannerAdapter::notifyNewMatch(Node *node) {
    start();
    for (auto it = listeners.begin(); it != listeners.end();) {
        activated = *it;
        activated->onNewXPathMatch(node);
        if (remove != 0) {
            it = listeners.erase(it);
        } else ++it;
    }
    stop();
}

void XPathScannerAdapter::notifyNewTree(BinaryTree *tree) {
    start();
    for (auto it = listeners.begin(); it != listeners.end();) {
        activated = *it;
        activated->onNewBinaryTree(tree);
        if (remove != 0) {
            it = listeners.erase(it);
        } else ++it;
    }
    stop();
}

void XPathScannerAdapter::notifyRemoveTree(BinaryTree *tree) {
    start();
    for (auto it = listeners.begin(); it != listeners.end();) {
        activated = *it;
        activated->onRemoveBinaryTree(tree);
        if (remove != 0) {
            it = listeners.erase(it);
        } else ++it;
    }
    stop();
}
