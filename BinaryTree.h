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
#include <set>
#include "XPathSyntaxTypes.h"
#include "BinaryTreeAdapter.h"
#include "Xpath.h"

class BinaryTree :
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
	Node * data = 0;
public:
	BinaryTree(BinaryTree *, const Xpath&);
	BinaryTree(const BinaryTree &, BinaryTree *);
	BinaryTree(const BinaryTree &, BinaryTree *, const id_type&, Node*);
	~BinaryTree();

	void optimize();
	void replaceDataScources(const std::set<NodeScanner *> &);

	void setData(Node*);
	Node * getData() const;
	void removeData();

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
	NodeScanner * getDataSource() const;
	std::set<NodeScanner *> getDataSources() const;
	const XPathSyntaxType& getType() const;
	const State& getState() const;
	id_type getIndex() const;

	void setIndex(const id_type&);
	void setSource(NodeScanner *);
	void setType(const XPathSyntaxType&);

	void printStructure() const;

	void stopDataContainingBranchesFromRecievingData();
	void stopSubBranchesFromRecievingData();

	void addListener(BinaryTreeListener*) override;
	void removeListener(BinaryTreeListener*) override;
};


#endif /* FILTERINTERPRETER_H_ */
