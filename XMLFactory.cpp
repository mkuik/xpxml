// The parser created a new attribute/*
/* XPathToXML.cpp * * Created on: Jun 14, 2015 * Author: matthijs */

#include <sstream>
#include "XMLFactory.h"

XMLFactory::XMLFactory(const std::string &xpath): Factory(xpath) {
	factory = new FactoryNode(0, "root", "", VIRTUAL, FactoryNode::INIT);
	factoryRoot = factory;
    factory->setName("");
//	getDatabase().addListener(this);

//	std::cout << std::endl;
//	factoryRoot->print();
//	std::cout << std::endl;
}

XMLFactory::~XMLFactory() {
//	getDatabase().removeListener(this);
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
//	if(DEBUG) {
//		printMemoryInfoColum();
//		std::printf("xpath found %s\n", node->toString().c_str());
//	}
	if(Node *realNode = node->getNonVirtualParent()) {
		if(Node * node = factoryRoot->findByID(realNode->getID())) {
			FactoryNode * sector = static_cast<FactoryNode*>(node);
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
//	if(DEBUG) {
//		std::printf("new %s element\n", node->toString().c_str());
//	}
	emptyTrash();
	FactoryNode *fNode = new FactoryNode(factory, *node, FactoryNode::OPEN_IN_XML);
	fNode->addListener(this);
	factory = fNode;

  //  std::cout << std::endl;
   // factoryRoot->print();
  //  std::cout << std::endl;
}


void XMLFactory::onNewComment(Node *node) {
    SaxParserListener::onNewComment(node);
//    if(DEBUG) {
//        std::printf("new %s comment\n", node->toString().c_str());
//    }
}

// 1b
// The parser created a new attribute
void XMLFactory::onNewAttribute(Node *node) {
//	if(DEBUG) {
//		std::printf("new %s attribute\n", node->toString().c_str());
//	}
	FactoryNode *fNode = new FactoryNode(factory, *node, FactoryNode::CLOSED_IN_XML);
	fNode->addListener(this);
//
//    std::cout << std::endl;
//    factoryRoot->print();
//    std::cout << std::endl;
}

// 5
// The parser closed an element
void XMLFactory::onEndOfElement(Node *node) {
//	if(DEBUG) {
//		std::printf("end of %s element\n", node->toString().c_str());
//	}
	factory = factory->getParent();
	if(FactoryNode *sector = static_cast<FactoryNode*>(factoryRoot->findByID(node->getID()))) {
		sector->setParserState(FactoryNode::CLOSED_IN_XML);
		id_type maxID = sector->getMaxID();
		std::stringstream stream;
		factoryRoot->flushXML(stream, maxID);
		moveStream(stream);
	}

//    std::cout << std::endl;
//    factoryRoot->print();
//    std::cout << std::endl;
	/*
	for(auto it = bucket.begin(); it != bucket.end();) {
		FactoryNode * sector = *it;
		if(sec!sector->isStoredIndirectly()) {
			it = bucket.erase(it);
			delete sector;
		}
		else {
			++it;
		}
	}*/
}

// An XMLFactoryNode was used as output
void XMLFactory::stateChanged(Node *directory) {
	FactoryNode * sector = static_cast<FactoryNode *>(directory);
    if (DEBUG && false) {
        std::printf("factory %s state changed closed:%s stored:%s\n",
                    sector->toString().c_str(),
                    sector->isClosedInParser() ? "true" : "false",
                    sector->isStoredIndirectly() ? "true" : "false");
    }
	if (sector->isClosedInParser() && !sector->isStored()) {
		addToTrash(sector);
	}
}


void XMLFactory::onNewBinaryTree(BinaryTree *tree) {
//	if(DEBUG) {
//		std::printf("\n");
//		printMemoryInfoColum();
//		std::printf("CREATE TREE\n");
//		tree->printStructure();
//	}
}

void XMLFactory::onRemoveBinaryTree(BinaryTree *tree) {
	
//	if(DEBUG) {
//		printMemoryInfoColum();
//		std::printf("REMOVE TREE\n");
//		tree->printStructure();
//	}
}

double XMLFactory::getEfficiency() const {
	return nDelete / (double)nNew;
}

void XMLFactory::onNewNode(Node *node) {
	nNew++;
}

void XMLFactory::printMemoryInfoColum() const {
//	std::printf("%4u %.0f%% %5u\t", factoryRoot->getTypeCount(), getEfficiency() * 100, factory->getID());
}

void XMLFactory::factoryDestructor(Node *directory) {
    FactoryNode * sector = static_cast<FactoryNode *>(directory);
//    if(DEBUG) {
//        printMemoryInfoColum();
//        std::printf("DELETE FACTORY NODE %s\n", sector->toString().c_str());
//    }
    nDelete++;
}

void XMLFactory::removeFromTrash(FactoryNode * node) {
    if (DEBUG && false) {
        std::printf("remove from trash %s\n", node->toString().c_str());
    }
	SecureTrashBin::removeFromTrash(node);
	delete node;

}

void XMLFactory::addToTrash(FactoryNode * node) {
    if (DEBUG && false) {
        std::printf("add to trash %s\n", node->toString().c_str());
    }
	SecureTrashBin::addToTrash(node);
}
