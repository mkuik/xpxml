/*
 * xPath.h
 *
 *  Created on: Nov 17, 2014
 *      Author: matthijs
 */

#ifndef PATHFILTER_H_
#define PATHFILTER_H_

#include "NodeScanner.h"
#include "SaxParser.h"
#include "BinaryTree.h"
#include <vector>
#include <list>

#include "SecureTrashBin.h"
#include "BinaryTreeAdapter.h"
#include "XPathScannerAdapter.h"

class XPathScanner : public BinaryTreeListener,
					 public XPathScannerAdapter,
					 public SecureTrashBin<BinaryTree>,
					 public Name {
	std::vector<std::string> xpath;
	std::list<BinaryTree *> cases;
	std::list<NodeScanner*> locators;
	BinaryTree * blueprint = 0;
	BinaryTree * activated = 0;
public:
	XPathScanner(const std::string&);
	~XPathScanner();
	void onAllocatedToBranch(Directory *, Node*);
	void onRedirectedFromBranch(Directory *, Node*);
	void addTree(BinaryTree *);
	void removeFromTrash(BinaryTree *);
	void setAdapter(SaxParserAdapter *);
	size_t getSize() const;
	void removeUnusedCases();
private:
	void newAlpha(const char&);
	void newIndicator(const char&);
	void newSpace();
};

#endif /* PATHFILTER_H_ */
