// The parser created a new attribute/*
/* XPathToXML.cpp * * Created on: Jun 14, 2015 * Author: matthijs */

#include <sstream>
#include "XMLFactory.h"
#include "Globals.h"

XMLFactory::XMLFactory(const std::string &xpath): Factory(xpath), 
root(new FactoryNode(0, "root", "", VIRTUAL, FactoryNode::INIT)) {
	dir = root;
}

XMLFactory::~XMLFactory() {
	FactoryNode::setRecusiveLink(true);
	delete root;
}

void XMLFactory::setAdapter(SaxParserAdapter *parser) {
	parser->addListener(this);
    Factory::setAdapter(parser);
}

void XMLFactory::cleanup() {
    std::stringstream stream;
    root->flushXML(stream, root->getMaxID());
    root->close(stream);
    moveStream(stream);
    for(XPathScanner *collector : structures)
        collector->emptyTrash();
    emptyTrash();
}

size_t XMLFactory::size() const {
    return root->getTypeCount();
}

// 4
// XPath evaluation result
void XMLFactory::onNewXPathMatch(Node *node) {
	Factory::onNewXPathMatch(node);
	if(Node *realNode = node->getNonVirtualParent()) {
		if(FactoryNode *sector = root->findByID(realNode->getID())) {
			sector->setConfirmed(true);
			if(sector->getType() == ATTRIBUTE && sector->hasParent()) {
				sector->getParent()->setConfirmed(true);
			}
		}
	}
}

// 1a
// The parser created a new element
void XMLFactory::onNewElement(Node *node) {
	FactoryNode *fNode = new FactoryNode(dir, *node, FactoryNode::OPEN_IN_XML);
	fNode->addListener(this);
	dir = fNode;
	emptyTrash();
}


void XMLFactory::onNewComment(Node *node) {
    SaxParserListener::onNewComment(node);
}

// 1b
// The parser created a new attribute
void XMLFactory::onNewAttribute(Node *node) {
	FactoryNode *fNode = new FactoryNode(dir, *node, FactoryNode::CLOSED_IN_XML);
	fNode->addListener(this);
}

void XMLFactory::onEndOfFile() {
	deleteXPathScanners();
	cleanup();
}

// 5
// The parser closed an element
void XMLFactory::onEndOfElement(Node *node) {
	dir = dir->getParent();
}

double XMLFactory::getEfficiency() const {
	return nDelete / (double)nNew;
}

std::string XMLFactory::toString() const {
	std::stringstream s;
	s << nDelete << "/" << nNew << "=" << (int)(getEfficiency() * 100 + 0.5) << "%";
	return s.str();
}

void XMLFactory::onNewNode(Node *node) {
	nNew++;
}

void XMLFactory::removeFromTrash(FactoryNode * node) {
	SecureTrashBin::removeFromTrash(node);
	delete node; 
}

void XMLFactory::onFactoryNodeDeleted(void *source, id_type id) {
	nDelete++;
}

void XMLFactory::onFactoryNodeClosedInParser(void *source, id_type id) {
	id_type maxID = static_cast<FactoryNode*>(source)->getMaxID();
	std::stringstream stream;
	root->flushXML(stream, maxID);
	moveStream(stream);
}

void XMLFactory::onFactoryNodeClosedInOutput(void *source, id_type id) {
	stateChanged(static_cast<FactoryNode*>(source));
}

void XMLFactory::onFactoryNodeSourceDeleted(void *source, id_type id) {
	stateChanged(static_cast<FactoryNode*>(source));
}

// An XMLFactoryNode was used as output
void XMLFactory::stateChanged(FactoryNode * sector) {
	if(sector->getType() == ELEMENT && sector->isClosedInParser() && !sector->isStored()) {
		for(FactoryNode * child : sector->getChildren()) {
			stateChanged(child);
		}
		addToTrash(sector);
	}
	else if(sector->getType() == ATTRIBUTE && sector->hasParent() && !sector->getParent()->isStored()) {
		addToTrash(sector);
	}
}




