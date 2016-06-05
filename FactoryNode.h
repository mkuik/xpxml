/*
 * XMLFactoryNode.h
 *
 *  Created on: Jun 18, 2015
 *      Author: matthijs
 */

#ifndef XMLFACTORYNODE_H_
#define XMLFACTORYNODE_H_

#include "Node.h"
#include "FactoryContact.h"

class FactoryNode :
        public Node,
        public FactoryOutput,
        public ObjectTypeCounter,
        public DestructorListener {
public:
    enum State { INIT, OPEN_IN_XML, CLOSED_IN_XML };
private:
    State parserState = OPEN_IN_XML;
    State outputState = INIT;
	bool confirmed = false;
    bool stored = false;
	bool sealedAttributes = false;
	unsigned int nSubElements = 0;
public:
	static id_type factoryCount;
    FactoryNode(FactoryNode *parent, Node &node, const State &state);
    FactoryNode(FactoryNode *parent, const std::string &, const std::string &,
                const NodeType&, const State &state);
    virtual ~FactoryNode();
	void addChild(Node *) override;
    void setConfirmed(const bool&);
    void setStored(const bool&);
    void setParserState(const State&);
    void setOutputState(const State&);
	bool isConfirmed() const;
    bool isConfirmedIndirectly() const;
    bool isConfirmedFromParent() const;
    bool isStored() const;
    bool isStoredIndirectly() const;
    bool isClosedInOutput() const;
    bool isOpenInOutput() const;
    bool isClosedInParser() const;
    bool isCollectingDust() const;
    bool hasSubElements() const;
    virtual FactoryNode * getParent() const override;
    void flushXML(std::ostream&, const id_type&);
    void close(std::ostream&);
    void print() const;

    virtual void onDestruct(void *pVoid) override;

    virtual void addListener(FactoryInput *l) override;

    virtual void removeListener(FactoryInput *l) override;

    virtual bool hasListener(FactoryInput *l) override;

private:
    //void flushElementOpenTag(std::ostream&);
    //void flushElementCloseTag(std::ostream&);
    //void flushAttribute(std::ostream&);
    void flushIndentation(std::ostream&) const;
    //void flushElementSingle(std::ostream &os);
    void flushXMLsubnodes(std::ostream &os, const id_type &end);

public:
	virtual void setParent(Node *node) override;
};

#endif /* XMLFACTORYNODE_H_ */
