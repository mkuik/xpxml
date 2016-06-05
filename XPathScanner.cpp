/*
 * xPath.cpp
 *
 *  Created on: Nov 17, 2014
 *      Author: matthijs
 */

#include "XPathScanner.h"

void mergeIdenticalLocators(BinaryTree *, const std::list<NodeScanner *> &);
void merge(NodeScanner *, NodeScanner *);

XPathScanner::XPathScanner(const std::string& xpath_string) : Name("xpath scanner") {
	enum {
		NO_QUOTE, SINGLE_QUOTE, DOUBLE_QUOTE
	} quote = NO_QUOTE;
	for (char c : xpath_string) {
		switch (c) {
		case '=':
		case '>':
		case '!':
		case '<':
		case ']':
		case '[':
		case '(':
		case ')':
		case '@':
		case '/':
			if (quote == NO_QUOTE)
				newIndicator(c);
			else
				newAlpha(c);
			break;
		case ' ':
			if (quote == NO_QUOTE)
                newSpace();
			else
				newAlpha(c);
			break;
		case '"':
			newIndicator(c);
			switch (quote) {
			case NO_QUOTE:
				quote = DOUBLE_QUOTE;
				break;
			case DOUBLE_QUOTE:
				quote = NO_QUOTE;
				break;
			case SINGLE_QUOTE:
				break;
			}
			break;
		case '\'':
			newIndicator(c);
			switch (quote) {
			case NO_QUOTE:
				quote = SINGLE_QUOTE;
				break;
			case SINGLE_QUOTE:
				quote = NO_QUOTE;
				break;
			case DOUBLE_QUOTE:
				break;
			}
			break;
		case '|':
			throw "unison operator found";
		default:
			newAlpha(c);
			break;
		}
	}

	blueprint = new BinaryTree(0, xpath.begin(), xpath.end());
	locators = blueprint->getLocators();
//    blueprint->printStructure();
//    std::cout << std::endl;

	for (std::list<NodeScanner *>::iterator subject = locators.begin();
			subject != locators.end(); ++subject) {
		for (std::list<NodeScanner *>::iterator query = locators.begin();
			 query != locators.end();) {

			if (query != subject
				&& (*query)->isWildcard() == (*subject)->isWildcard()
				&& (*query)->compareDirectory(*subject)) {
				query = locators.erase(query);
			}
			else
				++query;
		}
	}


    mergeIdenticalLocators(blueprint, locators);
	addTree(new BinaryTree(*blueprint, 0));

}

void mergeIdenticalLocators(BinaryTree *branch,
                            const std::list<NodeScanner *> &array) {
	NodeScanner * allocated = branch->getDataCollector();
	if  (allocated) {
		for (NodeScanner *query : array) {
			if (query && allocated != query
				&& allocated->isWildcard() == query->isWildcard()
				&& allocated->compareDirectory(query)) {
				branch->setSource(query);
				merge(query, allocated);
				break;
			}
		}
	}
	if (branch->getRight()) mergeIdenticalLocators(branch->getRight(), array);
	if (branch->getLeft()) mergeIdenticalLocators(branch->getLeft(), array);
}

void merge(NodeScanner *keep, NodeScanner *remove) {
    for(NodeScanner * child: remove->getChildren()) child->setParent(keep);
	if(remove->hasParent()) {
		remove->getParent()->removeChild(remove);
		remove->getParent()->addChild(keep);
	}
}

XPathScanner::~XPathScanner() {
	for (auto it = cases.begin(); it != cases.end();) {
		BinaryTree * tree = *it;
		it = cases.erase(it);
		delete tree;
	}

	std::list<NodeScanner *> locators = blueprint->getLocators();
	locators.sort();
	locators.unique();

	delete blueprint;

	for (auto it = locators.begin(); it != locators.end();) {
		NodeScanner * node = *it;
		it = locators.erase(it);
		delete node;
	}
}

void XPathScanner::onAllocatedToBranch(Directory * source, Node* ) {
	BinaryTree * tree = static_cast<BinaryTree *>(source->getTopParent());
	//std::printf("ALLOCATED TO BRANCH\n");
	//tree->printStructure();
	
	if (activated == 0) activated = tree;
	if (tree->iscomplete()) return;
    Node * data = tree->test();

    if(data) notifyNewMatch(data);
	

//	std::printf("CASES IN MEMORY\n");
//	for (BinaryTree * tree : cases) {
//		tree->printStructure();
//	}

	emptyTrash(activated);
}

void XPathScanner::onRedirectedFromBranch(Directory * source, Node* node) {
    BinaryTree *activatedBrach = static_cast<BinaryTree *>(source);
	activated = static_cast<BinaryTree *>(source->getTopParent());
    activated->deactivateFilledBranches();
    BinaryTree *newTree = new BinaryTree(*activated, 0, activatedBrach->getDataCollector()->getID(), node);
    
	removeUnusedCases();
	
	addTree(newTree);
	onAllocatedToBranch(newTree, node);
	activated = 0;
	//std::printf("NODE REDIRECTED\n");
}

void XPathScanner::addTree(BinaryTree *tree) {
	notifyNewTree(tree);
	tree->addListener(this);
	cases.push_back(tree);
}

void XPathScanner::removeUnusedCases() {
	for(auto it = cases.begin(); it != cases.end(); ) {
		BinaryTree * tree = *it;
		if(tree->iscomplete()) {
			it = cases.erase(it);
			addToTrash(tree);
		}
		else {
			++it;
		}
	}
}

void XPathScanner::removeFromTrash(BinaryTree *tree) {
    SecureTrashBin::removeFromTrash(tree);
	cases.remove(tree);
	//std::printf("DELETE TREE\n");
	//tree->printStructure();
	notifyRemoveTree(tree);
    delete tree;
}

void XPathScanner::setAdapter(SaxParserAdapter *parser) {
	for (NodeScanner * loc : locators)
        if (!parser->hasListener(loc))
            parser->addListener(loc);
}

size_t XPathScanner::getSize() const {
	return cases.size();
}

void XPathScanner::newAlpha(const char& c) {
	if (xpath.empty()) xpath.push_back(std::string(1, c));
	else xpath.back() += c;
}

void XPathScanner::newIndicator(const char& c) {
	if (!xpath.empty() && xpath.back().empty()) xpath.back() += c;
	else xpath.push_back(std::string(1, c));
	xpath.push_back("");
}

void XPathScanner::newSpace() {
	if (!xpath.empty() && !xpath.back().empty()) xpath.push_back("");
}