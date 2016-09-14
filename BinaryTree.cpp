/*
 * xPathBinaryEvalNode.cpp
 *
 *  Created on: Nov 21, 2014
 *      Author: matthijs
 */

#include "BinaryTree.h"
#include "XPathCommandDeconstructor.h"
#include "convert.h"
#include <string>

// BinaryTree constuctor from Xpath object
BinaryTree::BinaryTree(BinaryTree * ben, const Xpath& xpath) :
		Directory(ben) {

	setParent(ben);
	XPathCommandDeconstructor factory = XPathCommandDeconstructor(xpath);
    while (factory.getSyntaxType() == CONTAINER && factory.hasLeft()) {
        factory = XPathCommandDeconstructor(factory.getLeft());
    }
	Directory::setName(factory.getName());
	type = factory.getSyntaxType();

	if (type == NODE) {
		BinaryTree *p = ben;
		while (p && p->getDataSource() == 0) p = p->getParent();
		NodeScanner * parent = p ? p->getDataSource() : 0;
		source = new NodeScanner(parent, factory.getNodeType(), factory.getNodePathLink());
		source->setName(Directory::getName());
	} else if (type == VALUE) {
		if(!hasComparisonBeforeNodeParent()) {
			setType(INDEX);
		}
		else {
			Node *value = new Node("VALUE", Directory::getName(), 0, VIRTUAL);
			setData(value);
		}
	}

	if(factory.hasLeft()) left = new BinaryTree(this, factory.getLeft());
	if(factory.hasRight()) right = new BinaryTree(this, factory.getRight());
    
}

// BinaryTree copy constuctor. Does not copy allocated data
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
	if (type == VALUE) setData(obj.getData());

}

// BinaryTree copy constructor. Also copy's allocated data. When the target 
// branch is reached the allocated data is replaced with 'node'. All connected
// branches to the target do not copy allocated data.
BinaryTree::BinaryTree(const BinaryTree & obj, BinaryTree * p, const id_type& target, Node * node) :
		Directory(obj) {
	setParent(p);
	type = obj.type;

	if(obj.source) {
		source = obj.source;
		source->addListener((NodeScannerListener*)this);
	}

	if (getID() == target || (source && source->getID() == target)) {
		setData(node);
		setIndex(obj.getIndex() + 1);
		if (obj.left) left = new BinaryTree(*(obj.left), this);
		if (obj.right) right = new BinaryTree(*(obj.right), this);
	} else {
		if (obj.getData()) setData(obj.getData());
		if (obj.left) left = new BinaryTree(*(obj.left), this, target, node);
		if (obj.right) right = new BinaryTree(*(obj.right), this, target, node);
	}

	if (type == VALUE) setData(obj.getData());
}

// BinaryTree deconstructor
BinaryTree::~BinaryTree() {

	if (source) source->removeListener(this);
	if (left) delete left;
	if (right) delete right;

	removeData();
}

void BinaryTree::optimize() {
	auto locators = getDataSources();
	for(auto A = locators.begin(); A != locators.end(); ++A) {
		for(auto B = locators.begin(); B != locators.end();) {
			if(B != A && (*B)->isWildcard() == (*A)->isWildcard() && (*B)->cmpDir(*A))
				B = locators.erase(B);
			else ++B;
		}
	}
	replaceDataScources(locators);
}

void BinaryTree::replaceDataScources(const std::set<NodeScanner *> &array) {
	NodeScanner * allocated = getDataSource();
	if(getDataSource()) {
		for(NodeScanner *query : array) {
			if(query && allocated != query
				&& allocated->isWildcard() == query->isWildcard()
				&& allocated->cmpDir(query)) {
				setSource(query);
				merge(query, allocated);
				break;
			}
		}
	}
	if(getRight()) getRight()->replaceDataScources(array);
	if(getLeft()) getLeft()->replaceDataScources(array);
}

// Compare two strings. If both strings represent numerals then the are 
// compared as such.
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

// Evaluate the allocated data to the xpath conditions
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
			if (getData()) {
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
					C = getData();
				}
			}
			break;
		case VALUE:
			if (getData()) {
				C = getData();
			}
			break;
		case INDEX:
			if(memory) {
				if(memory->getData()) {
					if(memory->getIndex() == convert<id_type>(Directory::getName())) {
						C = memory->getData();
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
			C = memory->getData();
			break;
		case FUNC_STRING_LENGTH:
			if (!getData() && memory->getData()) {
				std::stringstream value;
				value << memory->getData()->getValue().length();
				setData(new Node("length(" + memory->getData()->getValue() + ")", value.str(), memory->getData()));
			}
			C = getData();
			break;
		case FUNC_INDEX:
			if (!getData() && memory->getData()) {
				std::stringstream value;
				value << memory->getIndex();
				setData(new Node("position(" + memory->getName() + ")", value.str(), memory->getData()));
			}
			C = getData();
			break;
		case UNION:
			break;
		default:
			break;
	}

	if (C) 
		state = TRUE;
	return C;
}

// Allocate data recieved from NodeScanner. If the branch already has allocated
// data or the data can't not be a member then the new data is redirected to 
// the BinaryTree container XPathScanner
void BinaryTree::onFoundNode(Node *node) {
	if (getData() == 0 && ismember(node)) {
		setData(node);
		notifyAllocation(this, node);
	} else {
		notifyRedirect(this, node);
		stopSubBranchesFromRecievingData();
	}
}

// If the sax parser closes the allocated element while sub branches are 
// missing data, then the branch sub section does not match the xpath
void BinaryTree::onNodeClosed() {
	if (!isfilled()) setFalseState();
}

// Allocate data to branch and a listener to the data. The data will notify the
// branch when the node is closed in case of an element.
void BinaryTree::setData(Node *node) {
	if(node) {
		data = node;
		data->notifyLink();
		data->addListener(this);
	}
}

// Check if all sub branches have allocated data where needed
bool BinaryTree::isfilled() const {
	return !(getType() == NODE && getData() == 0 || getLeft() && !getLeft()->isfilled() || getRight() && !getRight()->isfilled());
}

// Check if a data candidate is a subnode of previously allocated data
bool BinaryTree::ismember(Node * node) const {
	if (type == NODE) {
		if (getData()) {
			return getData()->getID() == node->getID();
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

// Collect all NodeScanners connected to subbranches
std::set<NodeScanner *> BinaryTree::getDataSources() const {
	std::set<NodeScanner *> list;
	if (source) list.insert(source);
	if(left) {
		auto l = left->getDataSources();
		list.insert(l.begin(), l.end());
	}
	if(right) {
		auto l = right->getDataSources();
		list.insert(l.begin(), l.end());
	}
	return list;
}

// Get left branch
BinaryTree * BinaryTree::getLeft()const {
	return left;
}

// Get right branch
BinaryTree * BinaryTree::getRight()const {
	return right;
}

// Add a listener to it's self and it's subbranches
void BinaryTree::addListener(BinaryTreeListener * listener) {
	BinaryTreeAdapter::addListener(listener);
	if (left) left->addListener(listener);
	if (right) right->addListener(listener);
}

// Remove a listener from it's self and it's subbranches
void BinaryTree::removeListener(BinaryTreeListener * listener) {
	BinaryTreeAdapter::removeListener(listener);
	if (right) right->removeListener(listener);
	if (left) left->removeListener(listener);
}

// Check wether a conclusion has been drawn by the test function
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

// Mark a branch section as false and reevaluate it's parent banch
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

// Get parent branch
BinaryTree *BinaryTree::getParent() const {
	return static_cast<BinaryTree *>(Directory::getParent());
}

// Get the type of syntax th branch represents from xpath
const XPathSyntaxType&BinaryTree::getType() const {
	return type;
}

// Get the connected NodeScanner
NodeScanner *BinaryTree::getDataSource() const {
	return source;
}

// Allocate a NodeScanner
void BinaryTree::setSource(NodeScanner * source) {
	this->source = source;
}

// Get the last test result of this branch
const BinaryTree::State& BinaryTree::getState() const {
	return state;
}

// Get the xml index the data represents. This is counted by the data redirect 
// system
id_type BinaryTree::getIndex() const {
	return index;
}

// Set the index
void BinaryTree::setIndex(const id_type &i) {
	index = i;
}

// Set the xpath syntax type this branch represents
void BinaryTree::setType(const XPathSyntaxType &t) {
	type = t;
}

// Stop the branches with allocated data from recieving new data
void BinaryTree::stopDataContainingBranchesFromRecievingData() {
    if (getDataSource() && getData()) getDataSource()->removeListener(this);
    if (left) left->stopDataContainingBranchesFromRecievingData();
    if (right) right->stopDataContainingBranchesFromRecievingData();
}

// Stop all branches from recieving new data
void BinaryTree::stopSubBranchesFromRecievingData() {
	if(getDataSource()) {
		if(getDataSource()->getConnectionType() == NodeScanner::EDGE_ROOT) return;
		else getDataSource()->removeListener(this);
	}
	if (left) left->stopSubBranchesFromRecievingData();
	if (right) right->stopSubBranchesFromRecievingData();
}

// Check wether the branch has a comparison as parent before any node parent 
// branches
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

// Print a visual representation of the BinaryTree with it's allocated data
void BinaryTree::printStructure() const {
	const int WIDTH = 30;

	if (!hasParent()) {
		std::cout << ".";
		for (int i = -2; i != WIDTH; ++i) std::cout << "-";
		std::cout << std::endl;
	}

	std::stringstream scanner, data;
	std::string depth(getDepth() * 3, ' ');
	data << Directory::getName();

	if(getData()) {
		data << ":" << getData()->getName() << "(" << getData()->getID() << ")";
		if(getData()->getType() != ELEMENT) {
			data << "=" << getData()->getValue();
		}
	}
	
	if(getDataSource()) {
		scanner << getDataSource()->getID();
	}

	std::printf("| %10s %s%s\n", 
		scanner.str().data(), 
		depth.c_str(), 
		data.str().data());

	if (left) left->printStructure();
	if (right) right->printStructure();

	if (!hasParent()) {
		std::cout << "'";
		for (int i = -2; i != WIDTH; ++i) std::cout << "-";
		const std::vector<const char*> translation = {"TRUE", "FALSE", "NA"};
		std::cout << " " << translation.at(state) << std::endl;
	}
}

// Get allocated data
Node * BinaryTree::getData() const {
	return data;
}

// Remove allocated data
void BinaryTree::removeData() {
	if(data) {
		data->removeListener(this);
		data->notifyUnlink();
		if(data->getLink() == 0) delete data;
		data = 0;
	}
}




