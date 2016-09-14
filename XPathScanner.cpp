/*
 * xPath.cpp
 *
 *  Created on: Nov 17, 2014
 *      Author: matthijs
 */

#include "XPathScanner.h"

// XPathScanner constructor. Here the BinaryTree blueprint is created from an 
// xpath. Identical NodeScanners are combined for efficiency.
XPathScanner::XPathScanner(const Xpath& x) : 
	xpath(x), blueprint(new BinaryTree(0, xpath)) {
	blueprint->optimize();
	//blueprint->printStructure();
	addTree(new BinaryTree(*blueprint, 0));
}

// XPathScanner deconstructor
XPathScanner::~XPathScanner() {
	for (auto it = cases.begin(); it != cases.end();) {
		BinaryTree * tree = *it;
		it = cases.erase(it);
		delete tree;
	}
	auto locators = blueprint->getDataSources();
	delete blueprint;
	emptyTrash();
	for (auto it = locators.begin(); it != locators.end();) {
		NodeScanner * node = *it;
		it = locators.erase(it);
		delete node;
	}
}

// Called when new data is allocated to a branch. Here the BinaryTree is 
// evaluated and results are sent to the containing Factory object.
void XPathScanner::onAllocatedToBranch(Directory * source, Node* ) {
	BinaryTree * tree = static_cast<BinaryTree *>(source->getTopParent());
	if (activated == 0) activated = tree;
	if (tree->iscomplete()) return;
    Node * data = tree->test();
    if(data) notifyNewMatch(data);
	emptyTrash(activated);
}

// Called when new data is sent to a branch that already has allocated data.
// Here the BinaryTree is copied and the new data is allocated to the
// redirecting branch position in the new BinaryTree.
void XPathScanner::onRedirectedFromBranch(Directory * source, Node* node) {
	activated = static_cast<BinaryTree *>(source->getTopParent());
	if(activated == cases.back()) {
		BinaryTree *newTree = new BinaryTree(*activated, 0, 
			static_cast<BinaryTree *>(source)->getDataSource()->getID(), node);
		removeUnusedCases();
		addTree(newTree);
		onAllocatedToBranch(newTree, node);
	} 
	activated = 0;
}

// Add a BinaryTree to the collection and start listening for branch updates.
void XPathScanner::addTree(BinaryTree *tree) {
	notifyNewTree(tree);
	tree->addListener(this);
	cases.push_back(tree);
}

// Check if any BinaryTrees have drawn an true or false conclusion. add those
// structures to the trashbin
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

// Delete a BinaryTree from the trashbin. Activated by the emptyTrash function.
void XPathScanner::removeFromTrash(BinaryTree *tree) {
    SecureTrashBin::removeFromTrash(tree);
	cases.remove(tree);
	notifyRemoveTree(tree);
    delete tree;
}

// Link the NodeScanner to a data source
void XPathScanner::setAdapter(SaxParserAdapter *p) {
	parser = p;
	for (NodeScanner * loc : blueprint->getDataSources())
        if (!parser->hasListener(loc))
            parser->addListener(loc);
}

// Get the number of BinaryTrees under evaluations
size_t XPathScanner::getSize() const {
	return cases.size();
}