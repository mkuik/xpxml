// The parser created a new attribute/*
/* XPathToXML.cpp * * Created on: Jun 14, 2015 * Author: matthijs */

#include <sstream>
#include "XMLFactory.h"

XMLFactory::XMLFactory(const std::string &xpath): Factory(xpath) {
	factory = new FactoryNode(0, "root", "", VIRTUAL, FactoryNode::INIT);
	factoryRoot = factory;
    factory->setName("");
}

XMLFactory::~XMLFactory() {
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

// 4
// XPath evaluation result
void XMLFactory::onNewXPathMatch(Node *node) {
	Factory::onNewXPathMatch(node);
	if(Node *realNode = node->getNonVirtualParent()) {
		if(FactoryNode *sector = factoryRoot->findByID(realNode->getID())) {
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
	emptyTrash();
	FactoryNode *fNode = new FactoryNode(factory, *node, FactoryNode::OPEN_IN_XML);
	fNode->addListener(this);
	factory = fNode;
}


void XMLFactory::onNewComment(Node *node) {
    SaxParserListener::onNewComment(node);
}

// 1b
// The parser created a new attribute
void XMLFactory::onNewAttribute(Node *node) {
	FactoryNode *fNode = new FactoryNode(factory, *node, FactoryNode::CLOSED_IN_XML);
	fNode->addListener(this);
}

// 5
// The parser closed an element
void XMLFactory::onEndOfElement(Node *node) {
	factory = factory->getParent();
	if(FactoryNode *sector = factoryRoot->findByID(node->getID())) {
		sector->setParserState(FactoryNode::CLOSED_IN_XML);
		id_type maxID = sector->getMaxID();
		std::stringstream stream;
		factoryRoot->flushXML(stream, maxID);
		moveStream(stream);
	}
}

// An XMLFactoryNode was used as output
void XMLFactory::stateChanged(Node *directory) {
	FactoryNode * sector = static_cast<FactoryNode *>(directory);
	if (sector->getType() == ELEMENT && sector->isClosedInParser() && !sector->isStored()) {
		for (FactoryNode * child : sector->getChildren()) {
			child->notifyChange(child);
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

void XMLFactory::factoryDestructor(Node *directory) {
    nDelete++;
}

void XMLFactory::removeFromTrash(FactoryNode * node) {
	SecureTrashBin::removeFromTrash(node);
	delete node;
}
