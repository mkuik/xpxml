//
// Created by Matthijs Kuik on 11/11/15.
//

#include "SaxParserAdapter.h"

void SaxParserAdapter::notifyNewNode(Node* node) {
    start();
    for(auto it = listeners.begin(); it != listeners.end();) {
        activated = *it;
        activated->onNewNode(node);
        if(remove != 0) {
            it = listeners.erase(it);
        }
        else ++it;
    }
    stop();
}

void SaxParserAdapter::notifyNewElement(Node * node) {
    start();
    for(auto it = listeners.begin(); it != listeners.end();) {
        activated = *it;
        activated->onNewElement(node);
        if(remove != 0) {
            it = listeners.erase(it);
        }
        else ++it;
    }
    stop();
}

void SaxParserAdapter::notifyNewAttribute(Node * node) {
    start();
    for(auto it = listeners.begin(); it != listeners.end();) {
        activated = *it;
        activated->onNewAttribute(node);
        if(remove != 0) {
            it = listeners.erase(it);
        }
        else ++it;
    }
    stop();
}

void SaxParserAdapter::notifyEndOfElement(Node * node) {
    start();
    for(auto it = listeners.begin(); it != listeners.end();) {
        activated = *it;
        activated->onEndOfElement(node);
        if(remove != 0) {
            it = listeners.erase(it);
        }
        else ++it;
    }
    stop();
}

void SaxParserAdapter::notifyComment(Node * node) {
    start();
    for(auto it = listeners.begin(); it != listeners.end();) {
        activated = *it;
        activated->onNewComment(node);
        if(remove != 0) {
            it = listeners.erase(it);
        }
        else ++it;
    }
    stop();
}