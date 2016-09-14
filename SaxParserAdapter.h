/*
 * xSaxListener.h
 *
 *  Created on: Nov 17, 2014
 *      Author: matthijs
 */

#ifndef SAXPARSEREVENTS_H_
#define SAXPARSEREVENTS_H_

#include "Node.h"
#include "Adapter.h"

class SaxParserListener {
public:
	~SaxParserListener() {}
	virtual void onNewElement(Node *node) {}
	virtual void onEndOfElement(Node *node) {}
	virtual void onNewAttribute(Node *node) {}
	virtual void onNewNode(Node *node) {}
	virtual void onNewComment(Node *node) {}
	virtual void onEndOfFile() {}
};

class SaxParserAdapter : public Adapter<SaxParserListener> {
public:
	void notifyNewNode(Node* node);
	void notifyNewElement(Node * node);
	void notifyNewAttribute(Node * node);
	void notifyEndOfElement(Node * node);
	void notifyComment(Node * node);
	void notifyEndOfFile();
};

#endif /* SAXPARSEREVENTS_H_ */
