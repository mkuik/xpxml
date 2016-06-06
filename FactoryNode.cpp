/*
 * XMLFactoryNode.cpp
 *
 *  Created on: Jun 18, 2015
 *      Author: matthijs
 */

#include "FactoryNode.h"
#include "Globals.h"

id_type FactoryNode::factoryCount = 0;
std::list<FactoryNode*> FactoryNode::objects;

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

	objects.push_back(this);
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

	objects.push_back(this);
}

FactoryNode::~FactoryNode() {
//	std::printf("DEL %p %s\n", (void*) this, toString().data());
	factoryCount--;
	for (FactoryNode *subnode : getChildren()) subnode->clearParent();
	if (hasParent()) getParent()->removeChild(this);

	objects.remove(this);
}

void FactoryNode::setParent(Node *node) {
	Directory::setParent(node);
}

void FactoryNode::addChild(FactoryNode * child) {
	Children::addChild(child);
	if(child->getType() == ELEMENT) ++nSubElements;
}


void FactoryNode::removeChild(FactoryNode *c) {
    Children::removeChild(c);
}

FactoryNode *FactoryNode::getParent() const {
	return static_cast<FactoryNode *>(Parent::getParent());
}

id_type FactoryNode::getMaxID() const {
	if(isParent())
		return getLastChild()->getMaxID();
	else
		return getID();
}

bool FactoryNode::isConfirmed() const {
	return confirmed;
}

bool FactoryNode::isConfirmedIndirectly() const {
	if(isConfirmed()) {
		return true;
	}
	else {
		for(const FactoryNode * child : getChildren())
			if(child->isConfirmedIndirectly())
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
		for(const FactoryNode * child : getChildren())
			if(child->isStoredIndirectly())
				return true;
		return false;
	}
}

bool FactoryNode::isClosedInParser() const {
	return parserState == CLOSED_IN_XML;
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
			for(FactoryNode *subnode : getChildren()) {
				if(subnode->getType() == ATTRIBUTE) {
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
		for(FactoryNode *subnode : getChildren()) {
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
		for(FactoryNode *subnode : getChildren()) {
			if(subnode->getType() != ATTRIBUTE) {
				subnode->close(os);
			}
		}
	}
}

void FactoryNode::flushXMLsubnodes(std::ostream &os, const id_type &end) {
	for(FactoryNode *subnode : getChildren()) {
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

FactoryNode *FactoryNode::findByID(const id_type &id) const {
	if(id == getID()) return (FactoryNode *) this;
	else {
		for(auto it = children.rbegin(); it != children.rend(); ++it) {
			if((*it)->getID() <= id) return (*it)->findByID(id);
		}
		return nullptr;
	}
}

void FactoryNode::print() const {
	if (!hasParent()) std::printf(" stored-confirmed-open, 'x'=true '.'=false '-'=na\n");
	const char * ENUM_STR = "-x.";
    std::printf(" %c%c%c %s%s\n", stored?'x':'.', confirmed?'x':'.', ENUM_STR[parserState],
				std::string(getDepth() * 2, ' ').c_str(), toString().c_str());
	for(FactoryNode *subnode : getChildren()) subnode->print();

}

void FactoryNode::onDestruct(void * pointer) {
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

void FactoryNode::notifyChangeInGroup() {
	for (FactoryNode * child : getChildren()) {
		child->notifyChangeInGroup();
	}
	notifyChange(this);
}


