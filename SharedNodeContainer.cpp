//
// Created by Matthijs Kuik on 15-11-2015.
//

#include "SharedNodeContainer.h"

SharedNodeContainer::SharedNodeContainer() {

}

SharedNodeContainer::~SharedNodeContainer() {

}

Node *SharedNodeContainer::getNode() const {
    return data;
}

void SharedNodeContainer::setNode(Node *node) {
    if (node) {
        data = node;
        data->notifyLink();
//        std::cout << "set node " << node->toString() << " " << node->getValue() <<  "\n";
    }
}

void SharedNodeContainer::removeNode() {
    if (data) {
        data->notifyUnlink();
        data = 0;
    }
}
