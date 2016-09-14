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
#include <set>

#include "SecureTrashBin.h"
#include "BinaryTreeAdapter.h"
#include "XPathScannerAdapter.h"

class XPathScanner : public BinaryTreeListener,
					 public XPathScannerAdapter,
					 public SecureTrashBin<BinaryTree> {
	const Xpath xpath;
	BinaryTree * blueprint;
	std::list<BinaryTree *> cases;
	SaxParserAdapter * parser = 0;
	BinaryTree * activated = 0;
public:
	XPathScanner(const Xpath&);
	~XPathScanner();
	void onAllocatedToBranch(Directory *, Node*);
	void onRedirectedFromBranch(Directory *, Node*);
	void addTree(BinaryTree *);
	void removeFromTrash(BinaryTree *);
	void setAdapter(SaxParserAdapter *);
	size_t getSize() const;
	void removeUnusedCases();
};

#endif /* PATHFILTER_H_ */
