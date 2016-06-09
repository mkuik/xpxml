/*
 * xNode.cpp
 *
 *  Created on: Nov 17, 2014
 *      Author: matthijs
 */

#include "Node.h"
#include "Globals.h"
#include <sstream>

id_type Node::nodeCount = 0;

Node::Node(Node *p, NodeType t) : Directory(p) {
    setType(t);
    nodeCount++;
}

Node::Node(const std::string &name, const std::string &value, Node *parent,
           NodeType origin) :
        Directory(parent, name), Value(value) {
    setType(origin);
    nodeCount++;
}

Node::Node(const Node &node) : Directory(node), ID(node), Value(node) {
    setType(node.type);
    nodeCount++;
}

Node::~Node() {
    nodeCount--;
    notifyNodeDestruct();
}

std::string Node::toString() const {
    std::stringstream s;
    s << getID() << ":" << getName() << "=" << getValue();
    return s.str();
}

void Node::setParent(Node *p) {
	if(p != getParent()) {
		Directory::setParent(p);
	}
}

Node *Node::getParent() const {
    return static_cast<Node *>(Directory::getParent());
}

NodeType Node::getType() const {
    return type;
}

void Node::setType(const NodeType &t) {
    type = t;
}

unsigned int Node::getDepth() const {
    if (hasParent())
        return getParent()->getDepth() + (getParent()->getType() != VIRTUAL ? 1 : 0);
    else
        return 0;
}

Node *Node::getNonVirtualParent() const {
    if (getType() != VIRTUAL) return (Node *) this;
    else if (hasParent())
        return getParent()->getNonVirtualParent();
    else return 0;
}

std::string Node::getPath() const {
    std::stringstream path;
    if (hasParent()) path << getParent()->getPath();
    path << "/";
    if (getType() == ATTRIBUTE) path << "@";
    path << getName() << "_" << getID();
    return path.str();
}

void Node::addListener(NodeListener *l) {
    Adapter::addListener(l);
}

unsigned short Node::getLink() const {
    return link;
}

void Node::notifyUnlink() {
    --link;
}

void Node::notifyLink() {
    ++link;
}

bool Node::isOpen() const {
    return open;
}

void Node::setOpen(bool open) {
    Node::open = open;
    notifyNodeClosed();
}





