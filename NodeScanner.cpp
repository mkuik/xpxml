/*
 * xNodeWrapper.cpp
 *
 *  Created on: Nov 19, 2014
 *      Author: matthijs
 */

#include <sstream>
#include "NodeScanner.h"

NodeScanner::NodeScanner(NodeScanner * n, NodeType o, Edge l) :
		Node(n, o), connection(l) {
	if (hasParent()) getParent()->addChild(this);
}

NodeScanner::~NodeScanner() {
	clearParent();
    clearChildren();
}

void NodeScanner::setName(const std::string& n) {
	Node::setName(n);
	if (n.compare("*") == 0) {
		wildcard = true;
	}
}

void NodeScanner::onNewNode(Node *node) {
//	std::cout << std::endl;
	if (node && compareDirectory(node)) {
		notifyFoundNode(node);
	}
}

bool NodeScanner::compareDirectory(Node *query, const NodeScanner *source) const {

	if (!query) {
		return false;
	} else if (query->getType() == getType() && (isWildcard() || getName().compare(query->getName()) == 0)) {
		switch (getConnectionType()) {
			case EDGE_ROOT:
				return true;
			case EDGE_ANCESTOR:
			case EDGE_DIRECT:
				return !hasParent() || getParent()->compareDirectory(query->getParent(), this);
		}
	} else if (source) {
		switch (source->getConnectionType()) {
			case EDGE_ROOT:
				return false;
			case EDGE_ANCESTOR:
				return compareDirectory(query->getParent(), source);
			case EDGE_DIRECT:
				return false;
		}
	}
	return false;
}

NodeScanner::Edge NodeScanner::getConnectionType() const {
	return connection;
}

std::string NodeScanner::toString() const {
	std::stringstream summary;
	summary << getID() << ":";
	switch (connection) {
		case EDGE_DIRECT:
			summary << "/";
			break;
		case EDGE_ANCESTOR:
			summary << "//";
			break;
		default:
			summary << "";
			break;
	}
	summary << getName();
	return summary.str();
}

bool NodeScanner::isWildcard() const {
	return wildcard;
}

NodeScanner *NodeScanner::getParent() const {
	return static_cast<NodeScanner *>(Parent::getParent());
}

void NodeScanner::setParent(NodeScanner * parent) {
	Node::setParent(parent);
}

void NodeScanner::addListener(NodeScannerListener *l) {
    NodeScannerAdapter::addListener(l);
}

void NodeScanner::removeListener(NodeScannerListener *l) {
    NodeScannerAdapter::removeListener(l);
}

bool NodeScanner::hasListener(NodeScannerListener *l) {
    return NodeScannerAdapter::hasListener(l);
}
