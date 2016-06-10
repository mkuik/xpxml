// The parser created a new attribute/*
/* XPathToXML.cpp * * Created on: Jun 14, 2015 * Author: matthijs */

#include <sstream>
#include "XMLFactory.h"
#include "Globals.h"

XMLFactory::XMLFactory(const std::string &xpath): Factory(xpath) {
	factoryRoot = factory = new FactoryNode(0, "root", "", VIRTUAL, FactoryNode::INIT);
}

XMLFactory::~XMLFactory() {
	deleteXPathScanners();
	cleanup();
	FactoryNode::setRecusiveLink(true);
	delete factoryRoot;
}

void XMLFactory::setAdapter(SaxParserAdapter *parser) {
	parser->addListener(this);
    Factory::setAdapter(parser);
}

void XMLFactory::cleanup() {
    std::stringstream stream;
    factoryRoot->flushXML(stream, factoryRoot->getMaxID());
    factoryRoot->close(stream);
    moveStream(stream);
    for(XPathScanner *collector : structures)
        collector->emptyTrash();
    emptyTrash();
}

size_t XMLFactory::size() const {
    return factoryRoot->getTypeCount();
}

// 1a
// The parser created a new element
void XMLFactory::onNewElement(Node *node) {
	FactoryNode *fNode = new FactoryNode(factory, *node, FactoryNode::OPEN_IN_XML);
	fNode->FactoryAdapter::addListener(this);
	factory = fNode;

	emptyTrash();
}

// 1b
// The parser created a new attribute
void XMLFactory::onNewAttribute(Node *node) {
	FactoryNode *fNode = new FactoryNode(factory, *node, FactoryNode::CLOSED_IN_XML);
	fNode->FactoryAdapter::addListener(this);
}

// 5
// The parser closed an element
void XMLFactory::onEndOfElement(Node *node) {
	factory = factory->getParent();
}

// An XMLFactoryNode was used as output
void XMLFactory::stateChanged(FactoryNode * sector) {
	if (sector->getType() == ELEMENT && sector->isClosedInParser() && !sector->isStored()) {
		for (FactoryNode * child : sector->getChildren()) {
			stateChanged(child);
		}
		addToTrash(sector);
	} else if (sector->getType() == ATTRIBUTE && sector->hasParent() && !sector->getParent()->isStored()) {
		addToTrash(sector);
	}
}

double XMLFactory::getEfficiency() const {
	return nDelete / (double)nNew;
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
	factoryRoot->flushXML(stream, maxID);
	moveStream(stream);
}

void XMLFactory::onFactoryNodeClosedInOutput(void *source, id_type id) {
	stateChanged(static_cast<FactoryNode*>(source));
}

void XMLFactory::onFactoryNodeSourceDeleted(void *source, id_type id) {
	stateChanged(static_cast<FactoryNode*>(source));
}






