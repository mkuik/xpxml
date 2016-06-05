/*
 * XMLFactoryNode.cpp
 *
 *  Created on: Jun 18, 2015
 *      Author: matthijs
 */

#include "FactoryNode.h"

id_type FactoryNode::factoryCount = 0;

FactoryNode::FactoryNode(FactoryNode *parent, Node &node, const State& state):
	Node(node), parserState(state) {
	setParent(parent);
	if(hasParent()) {
        getParent()->addChild(this);
    }
	if(getType() == INSTRUCTION) setConfirmed(true);

    setStored(true);
    node.addListener(this);
	factoryCount++;
}


FactoryNode::FactoryNode(FactoryNode *parent, const std::string &name,
                         const std::string &value, const NodeType &type,
                         const FactoryNode::State &state) :
    Node(name, value, parent, type), parserState(state) {
    if(hasParent()) {
        getParent()->addChild(this);
    }
    if(getType() == INSTRUCTION) setConfirmed(true);
	factoryCount++;
}

FactoryNode::~FactoryNode() {
	factoryCount--;
}

void FactoryNode::setParent(Node *node) {
	Directory::setParent(node);
}

void FactoryNode::addChild(Node * child) {
	Children::addChild(child);
	if(child->getType() == ELEMENT) ++nSubElements;
}

FactoryNode *FactoryNode::getParent() const {
	return static_cast<FactoryNode *>(Parent::getParent());
}

bool FactoryNode::isConfirmed() const {
	return confirmed;
}

bool FactoryNode::isConfirmedIndirectly() const {
	if(isConfirmed()) {
		return true;
	}
	else {
		for(Node * child : getChildren())
			if(static_cast<FactoryNode*>(child)->isConfirmedIndirectly())
				return true;
		return false;
	}
}

bool FactoryNode::isConfirmedFromParent() const {
	return isConfirmed() || (hasParent() && getParent()->isConfirmedFromParent());
}

bool FactoryNode::isStored() const {
	return stored;
}

bool FactoryNode::isStoredIndirectly() const {
	if(isStored()) {
		return true;
	}
	else {
		for(Node * child : getChildren())
			if(static_cast<FactoryNode*>(child)->isStoredIndirectly())
				return true;
		return false;
	}
}

bool FactoryNode::isClosedInOutput() const {
	return outputState == CLOSED_IN_XML;
}

bool FactoryNode::isOpenInOutput() const {
	return outputState == OPEN_IN_XML;
}

bool FactoryNode::isClosedInParser() const {
	return parserState == CLOSED_IN_XML;
}

bool FactoryNode::isCollectingDust() const {
	return isClosedInParser() && outputState != OPEN_IN_XML &&
		!isStoredIndirectly() && !isConfirmedIndirectly();
}

bool FactoryNode::hasSubElements() const {
	return nSubElements != 0;
}

void FactoryNode::setConfirmed(const bool &YN) {
	confirmed = YN;
}

void FactoryNode::setStored(const bool &YN) {
    stored = YN;
    notifyChange(this);
}

void FactoryNode::setParserState(const State &state) {
    if (parserState != state) {
        parserState = state;
        notifyChange(this);
    }
}

void FactoryNode::setOutputState(const State &state) {
    if (outputState != state) {
        outputState = state;
        notifyChange(this);
    }
}

void FactoryNode::flushXML(std::ostream &os, const id_type &end) {
	/*flushIndentation(os);
	std::printf("%s flushxml  confirmed:%s  end:%u\n", toString().c_str(),
		(isConfirmedIndirectly() ? "Y" : "N"), end);*/
	if(getType() == VIRTUAL) {
		flushXMLsubnodes(os, end);
	}
	else {
		if(outputState == INIT && (isConfirmedIndirectly() || isConfirmedFromParent())) {
			flushIndentation(os);
			os << '<';
			if(getType() == INSTRUCTION) os << '?';
			os << getName();
			for(Node * child : getChildren()) {
				FactoryNode * subnode = static_cast<FactoryNode*>(child);
				if(static_cast<FactoryNode*>(subnode)->getType() == ATTRIBUTE) {
					os << ' ' << subnode->getName() << "=\"" << subnode->getValue() << "\"";
					subnode->setOutputState(CLOSED_IN_XML);
				}
			}
			if(hasSubElements()) {
				os << ">\n";
				sealedAttributes = true;
			}
			setOutputState(OPEN_IN_XML);
			flushXMLsubnodes(os, end);
		}
		if(outputState == OPEN_IN_XML) {
			if(!sealedAttributes && hasSubElements()) {
				os << ">\n";
				sealedAttributes = true;
			}
			flushXMLsubnodes(os, end);
			if(parserState == CLOSED_IN_XML && getMaxID() <= end) {
				if(hasSubElements()) {
					flushIndentation(os);
					os << "</" << getName() << ">\n";
				}
				else if(getType() == INSTRUCTION) {
					os << "?>\n";
				}
				else {
					os << "/>\n";
				}
				setOutputState(CLOSED_IN_XML);
			}
			
		}
	}
}

void FactoryNode::close(std::ostream &os) {
	if(outputState == OPEN_IN_XML) {
		for(Node * child : getChildren()) {
			FactoryNode * subnode = static_cast<FactoryNode*>(child);
			if(subnode->getType() != ATTRIBUTE) {
				subnode->close(os);
			}
		}
		if(hasSubElements()) {
			flushIndentation(os);
			os << "</" << getName() << ">\n";
		}
		else os << "/>\n";
		setOutputState(CLOSED_IN_XML);
	}
	else if(getType() == VIRTUAL) {
		for(Node * child : getChildren()) {
			FactoryNode * subnode = static_cast<FactoryNode*>(child);
			if(subnode->getType() != ATTRIBUTE) {
				subnode->close(os);
			}
		}
	}
}

void FactoryNode::flushXMLsubnodes(std::ostream &os, const id_type &end) {
	for(Node * child : getChildren()) {
		FactoryNode * subnode = static_cast<FactoryNode*>(child);
		if(subnode->getType() != ATTRIBUTE) {
			if(subnode->getID() <= end) 
				subnode->flushXML(os, end);
			else break;
		}
	}
}

void FactoryNode::flushIndentation(std::ostream &os) const {
	//os << nodeCount << " " << factoryCount << " ~ " << nodeCount - factoryCount << " ";
	for(unsigned int i = getDepth(); i != 0; --i) os << "    ";
}



void FactoryNode::print() const {
	if (!hasParent()) std::printf(" stored-confirmed-open, 'x'=true '.'=false '-'=na\n");
	const char * ENUM_STR = "-x.";
    std::printf(" %c%c%c %s%s\n", stored?'x':'.', confirmed?'x':'.', ENUM_STR[parserState],
				std::string(getDepth() * 2, ' ').c_str(), toString().c_str());
	for(Node *subnode : getChildren()) static_cast<FactoryNode*>(subnode)->print();

}

void FactoryNode::onDestruct(void * pointer) {
//    std::printf("set stored false %s\n", toString().c_str());
    setStored(false);
}

void FactoryNode::addListener(FactoryInput *l) {
    FactoryOutput::addListener(l);
}

void FactoryNode::removeListener(FactoryInput *l) {
    FactoryOutput::removeListener(l);
}

bool FactoryNode::hasListener(FactoryInput *l) {
    return FactoryOutput::hasListener(l);
}
