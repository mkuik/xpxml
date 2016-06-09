/*
 * xPathBinaryEvalNode.h
 *
 *  Created on: Nov 21, 2014
 *      Author: matthijs
 */

#ifndef FILTERINTERPRETER_H_
#define FILTERINTERPRETER_H_

#include "Directory.h"
#include "NodeScanner.h"
#include <vector>
#include <list>
#include "XPathSyntaxTypes.h"
#include "BinaryTreeAdapter.h"
#include "SharedNodeContainer.h"

class BinaryTree : public SharedNodeContainer,
		public Directory,
		public ID,
		public NodeScannerListener,
		public NodeListener,
		public BinaryTreeAdapter {
public:
	enum State {
		TRUE, FALSE, NA
	};
private:
	State state = NA;
	BinaryTree * left = 0;
	BinaryTree * right = 0;
	NodeScanner * source = 0;
	XPathSyntaxType type = NODE;
	id_type index = 1;
public:
	virtual void setNode(Node *node) override;

	BinaryTree(BinaryTree *, std::vector<std::string>::iterator,
			std::vector<std::string>::iterator);
	BinaryTree(const BinaryTree &, BinaryTree *);
	BinaryTree(const BinaryTree &, BinaryTree *, const id_type&, Node*);
	~BinaryTree();


	virtual void removeNode() override;

	Node *test(BinaryTree * = 0);
	void setFalseState();

	void onFoundNode(Node *) override; // NodeScanner input
	void onNodeClosed() override; // Node input

	bool isfilled() const;
	bool ismember(Node *) const;
	bool iscomplete() const;
    bool hasComparisonBeforeNodeParent() const;

	BinaryTree * getParent() const override;
	BinaryTree * getLeft()const;
	BinaryTree * getRight()const;
	NodeScanner * getDataCollector();
	const XPathSyntaxType& getType() const;
	const State& getState() const;
	//std::list<Node*> getAllData() const;
	std::list<NodeScanner *> getLocators() const;
	id_type getIndex() const;

	void setIndex(const id_type&);
	void setSource(NodeScanner *);
	void setType(const XPathSyntaxType&);

	//void printStatus() const;
	void printStructure() const;

	void deactivateFilledBranches();
	void deactivateAll();

	void addListener(BinaryTreeListener*) override;
	void removeListener(BinaryTreeListener*) override;
};

#endif /* FILTERINTERPRETER_H_ */
