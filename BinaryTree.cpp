/*
 * xPathBinaryEvalNode.cpp
 *
 *  Created on: Nov 21, 2014
 *      Author: matthijs
 */

#include "BinaryTree.h"
#include "tools.h"
#include "XPathCommandDeconstructor.h"
#include <string>

BinaryTree::BinaryTree(BinaryTree * ben, std::vector<std::string>::iterator begin,
		std::vector<std::string>::iterator end) :
		Directory(ben) {

	setParent(ben);
	XPathCommandDeconstructor factory = XPathCommandDeconstructor(begin, end);
    while (factory.getSyntaxType() == CONTAINER && factory.hasLeft()) {
        factory = XPathCommandDeconstructor(factory.getLeftBegin(), factory.getLeftEnd());
    }
	Directory::setName(factory.getName());
	type = factory.getSyntaxType();

	if (type == NODE) {
		BinaryTree *p = ben;
		while (p && p->getDataCollector() == 0) p = p->getParent();
		NodeScanner * parent = p ? p->getDataCollector() : 0;
		source = new NodeScanner(parent, factory.getNodeType(), factory.getNodePathLink());
		source->setName(Directory::getName());
	} else if (type == VALUE) {
		if(!hasComparisonBeforeNodeParent()) {
			setType(INDEX);
		}
		else {
			Node *value = new Node("VALUE", Directory::getName(), 0, VIRTUAL);
			setNode(value);
		}
	}

	if(factory.hasLeft()) {
		left = new BinaryTree(this, factory.getLeftBegin(), factory.getLeftEnd());
	}
	if(factory.hasRight()) {
		right = new BinaryTree(this, factory.getRightBegin(), factory.getRightEnd());
	}
    
}

BinaryTree::BinaryTree(const BinaryTree & obj, BinaryTree * p) :
		Directory(obj) {

	setParent(p);
	type = obj.type;

	if(obj.source) {
		source = obj.source;
		source->addListener(this);
	}
	if (obj.left) left = new BinaryTree(*(obj.left), this);
	if (obj.right) right = new BinaryTree(*(obj.right), this);
	if (type == VALUE) setNode(obj.getNode());

}

BinaryTree::BinaryTree(const BinaryTree & obj, BinaryTree * p, const id_type& target, Node * node) :
		Directory(obj) {
	setParent(p);
	type = obj.type;

	if(obj.source) {
		source = obj.source;
		source->addListener((NodeScannerListener*)this);
	}

	if (getID() == target || (source && source->getID() == target)) {
		setNode(node);
		setIndex(obj.getIndex() + 1);
		if (obj.left) left = new BinaryTree(*(obj.left), this);
		if (obj.right) right = new BinaryTree(*(obj.right), this);
	} else {
		if (obj.getNode()) setNode(obj.getNode());
		if (obj.left) left = new BinaryTree(*(obj.left), this, target, node);
		if (obj.right) right = new BinaryTree(*(obj.right), this, target, node);
	}

	if (type == VALUE) setNode(obj.getNode());
}

BinaryTree::~BinaryTree() {

	if (source) source->removeListener(this);
	if (left) delete left;
	if (right) delete right;

	removeNode();
}


int compare(const std::string& A, const std::string& B) {
//	std::printf("compare('%s', '%s')\n", A.c_str(), B.c_str());
	try {
		const double numberA = convert<double>(A);
		const double numberB = convert<double>(B);
		if (numberA == numberB) {
			return 0;
		} else if (numberA < numberB) {
			return -1;
		} else {
			return 1;
		}
	} catch (...) {
		return A.compare(B);
	}
}

Node *BinaryTree::test(BinaryTree *memory) {
	if (getState() == FALSE) return 0;

	Node * A = 0; // left
	Node * B = 0; // right
	Node * C = 0; // return value

	switch (type) {
		case AND:
			if (left && right && (A = left->test(memory))
				&& (B = right->test(memory)))
				C = A;
			break;
		case OR:
			if ((left && (A = left->test(memory)))
				|| (right && (B = right->test(memory))))
				C = (A ? A : B);
			break;
		case XOR:
			if (left && right && ((A = left->test(memory)) != 0) != ((B =
																			  right->test()) != 0))
				C = (A ? A : B);
			break;
		case LESSER_OR_EQUAL:
			if (left && right && (A = left->test(memory))
				&& (B = right->test(memory))) {
				if (compare(A->getValue(), B->getValue()) <= 0)
					C = A;
				else
					setFalseState();
			}
			break;
		case GREATER_OR_EQUAL:
			if (left && right && (A = left->test(memory))
				&& (B = right->test(memory))) {
				if (compare(A->getValue(), B->getValue()) >= 0)
					C = A;
				else
					setFalseState();
			}
			break;
		case LESSER:
			if (left && right && (A = left->test(memory))
				&& (B = right->test(memory))) {
				if (compare(A->getValue(), B->getValue()) < 0)
					C = A;
				else
					setFalseState();
			}
			break;
		case GREATER:
			if (left && right && (A = left->test(memory))
				&& (B = right->test(memory))) {
				if (compare(A->getValue(), B->getValue()) > 0)
					C = A;
				else
					setFalseState();
			}
			break;
		case EQUAL:
			if (left && right && (A = left->test(memory))
				&& (B = right->test(memory))) {
				if (compare(A->getValue(), B->getValue()) == 0)
					C = A;
				else
					setFalseState();
			}
			break;
		case UNEQUAL:
			if (left && right && (A = left->test(memory))
				&& (B = right->test(memory))) {
				if (compare(A->getValue(), B->getValue()) != 0)
					C = A;
				else
					setFalseState();
			}
			break;
		case NOT:
			if (left && !(A = left->test(memory)))
				C = source;
			break;
		case CONTAINER:
			if ((left && (A = left->test(memory))) || (right && ((B = right->test(memory)))))
				C = (A ? A : B);
			break;
		case NODE:
			if (getNode()) {
				if (left && right) {
					if ((B = right->test(this)) && (A = left->test(this))) {
						C = A;
					}
				} else if (left) {
					if ((A = left->test(this))) {
						C = A;
					}
				}
				else if(right) {
					if((B = right->test(this))) {
						C = B;
					}
				}
				else {
					C = getNode();
				}
			}
			break;
		case VALUE:
			if (getNode()) {
				C = getNode();
			}
			break;
		case INDEX:
			if(memory) {
				if(memory->getNode()) {
					if(memory->getIndex() == convert<id_type>(Directory::getName())) {
						C = memory->getNode();
					}
					else {
						setFalseState();
					}
				}
			}
			else {
				setFalseState();
			}
			break;
		case FUNC_TEXT:
			C = memory->getNode();
			break;
		case FUNC_STRING_LENGTH:
			if (!getNode() && memory->getNode()) {
				std::stringstream value;
				value << memory->getNode()->getValue().length();
				C = new Node("length(" + memory->getNode()->getValue() + ")", value.str(), memory->getNode());
				setNode(C);
			}
			C = getNode();
			break;
		case FUNC_INDEX:
			if (!getNode() && memory->getNode()) {
				std::stringstream value;
				value << memory->getIndex();
				C = new Node("position(" + memory->getName() + ")", value.str(), memory->getNode());
				setNode(C);
			}
			C = getNode();
			break;
		case UNION:
			break;
		default:
			break;
	}

	if (C) {
		state = TRUE;
	}

//	std::printf("test %s -> %s\n", getPath().data(), C ? C->getPath().data() : "0");
	return C;
}

// Extract the value from node (if conditions from listeners are true)
void BinaryTree::onFoundNode(Node *node) {
	if (getNode() == 0 && ismember(node)) {
		setNode(node);
		notifyAllocation(this, node);
	} else {
		notifyRedirect(this, node);
	}
}


void BinaryTree::setNode(Node *node) {
	SharedNodeContainer::setNode(node);
//	printStructure();
}


bool BinaryTree::ismember(Node * node) const {
	if (type == NODE) {
		if (getNode()) {
			return getNode()->getID() == node->getID();
		} else if (hasParent() && source
				&& source->getConnectionType() == NodeScanner::EDGE_ANCESTOR) {
			for (Node * x = node->getParent(); x; x = x->getParent()) {
				if (getParent()->ismember(x)) {
					return true;
				}
			}
			return false;
		} else
			return !(hasParent() && node->getParent())
					|| getParent()->ismember(node->getParent());
	} else
		return hasParent() && getParent()->ismember(node);
}

std::list<NodeScanner *> BinaryTree::getLocators() const {
	std::list<NodeScanner *> list;
	if (source) list.push_back(source);
	if (left) list.merge(left->getLocators());
	if (right) list.merge(right->getLocators());
	return list;
}

BinaryTree *BinaryTree::getLeft() {
	return left;
}

BinaryTree *BinaryTree::getRight() {
	return right;
}

void BinaryTree::addListener(BinaryTreeListener * listener) {
	BinaryTreeAdapter::addListener(listener);
	if (left) left->addListener(listener);
	if (right) right->addListener(listener);
}

void BinaryTree::removeListener(BinaryTreeListener * listener) {
	BinaryTreeAdapter::removeListener(listener);
	if (right) right->removeListener(listener);
	if (left) left->removeListener(listener);
}

bool BinaryTree::iscomplete() const {
	switch (state) {
	case TRUE:
	case FALSE:
		return true;
	default:
		return false;
		break;
	}
}

void BinaryTree::setFalseState() {
	switch (type) {
		default:
			state = FALSE;
			if (hasParent())
				getParent()->setFalseState();
			break;
		case UNION:
		case OR:
			if (left->getState() == FALSE && right->getState() == FALSE) {
				state = FALSE;
				if (hasParent())
					getParent()->setFalseState();
			}
			break;
		case XOR:
			if (left->getState() != right->getState() && left->iscomplete()
				&& right->iscomplete()) {
				state = FALSE;
				if (hasParent())
					getParent()->setFalseState();
			}
			break;
		case NOT:
			break;
	}

}

BinaryTree *BinaryTree::getParent() const {
	return static_cast<BinaryTree *>(Directory::getParent());
}

const XPathSyntaxType&BinaryTree::getType() const {
	return type;
}

NodeScanner *BinaryTree::getDataCollector() {
	return source;
}

void BinaryTree::setSource(NodeScanner * source) {
	this->source = source;
}

const BinaryTree::State&BinaryTree::getState() const {
	return state;
}

id_type BinaryTree::getIndex() const {
	return index;
}

void BinaryTree::setIndex(const id_type &i) {
	index = i;
}

void BinaryTree::setType(const XPathSyntaxType &t) {
	type = t;
}

void BinaryTree::deactivateFilledBranches() {
    if (source && getNode()) source->removeListener(this);
    if (left) left->deactivateFilledBranches();
    if (right) right->deactivateFilledBranches();
}

void BinaryTree::deactivateAll() {
	if (source) source->removeListener(this);
	if (left) left->deactivateAll();
	if (right) right->deactivateAll();
}

bool BinaryTree::hasComparisonBeforeNodeParent() const {
    for (const BinaryTree * p = getParent(); p; p = p->getParent()) {
        switch(p->getType()) {
            case NODE: return false;
            case GREATER:
            case LESSER:
            case GREATER_OR_EQUAL:
            case LESSER_OR_EQUAL:
            case EQUAL:
            case UNEQUAL: return true;
            default: break;
        }
    }
    return false;
}

void BinaryTree::printStructure() const {
	const int WIDTH = 30;

	if (!hasParent()) {
		std::cout << ".";
		for (int i = -2; i != WIDTH; ++i) std::cout << "-";
//		std::cout << "." << std::endl;
		std::cout << std::endl;
	}

	std::stringstream sformat;
//	sformat << "| %-" << WIDTH << "s |";
	sformat << "| %s";
	std::string sdata(getDepth() * 3, ' ');
	if (getNode() != nullptr) sdata += Directory::getName() + ":" + getNode()->getName() + " " + (getNode()->getType() != ELEMENT ? getNode()->getValue() : "");
	else sdata += getName();
	std::printf(sformat.str().c_str(), sdata.c_str());
    if (getType() == NODE && getNode()) std::printf(" %4u %4i", getIndex(), getNode()->getID());
    std::printf("\n");

	if (left) left->printStructure();
	if (right) right->printStructure();

	if (!hasParent()) {
		std::cout << "'";
		for (int i = -2; i != WIDTH; ++i) std::cout << "-";
//		std::cout << std::endl;
		const std::vector<const char*> translation = {"TRUE", "FALSE", "NA"};
		std::cout << " " << translation.at(state) << std::endl;
	}
}

void BinaryTree::removeNode() {
	Node * node = getNode();
//	std::printf("DELETE %s BRANCH NODE\n", Directory::getName().data());
	SharedNodeContainer::removeNode();
	if (node && node->getLink() == 0) {
		delete node;
	}
}