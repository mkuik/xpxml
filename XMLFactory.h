/*
 * XPathToXML.h
 *
 *  Created on: Jun 14, 2015
 *      Author: matthijs
 */

#ifndef XPATHTOXML_H_
#define XPATHTOXML_H_

#include "Factory.h"
#include "SaxParserAdapter.h"
#include "FactoryNode.h"
#include "FactoryContact.h"

class XMLFactory : public Factory,
		public SaxParserListener,
        public FactoryInput,
		public SecureTrashBin<FactoryNode> {
	FactoryNode *factory, *factoryRoot;
	const bool DEBUG = true;
	id_type nNew = 0, nDelete = 0;
	std::list<FactoryNode*> bucket;
public:
	XMLFactory(const std::string&);
	~XMLFactory();
    void setAdapter(SaxParserAdapter *) override;
	void cleanup();
	size_t size() const override;
private:
	virtual void onNewComment(Node *node) override;
	void onNewXPathMatch(Node *) override;
	void onNewNode(Node *) override;
	void onNewElement(Node *) override;
	void onEndOfElement(Node *) override;
	void onNewAttribute(Node *) override;
    void stateChanged(Node *) override;
    void factoryDestructor(Node *) override;
    void removeFromTrash(FactoryNode*) override;
	double getEfficiency() const override;
};

#endif /* XPATHTOXML_H_ */