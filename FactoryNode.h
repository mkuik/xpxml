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
        public Children<FactoryNode>,
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
	static bool recusiveLink;
public:
	static id_type factoryCount;
    FactoryNode(FactoryNode *parent, Node &node, const State &state);
    FactoryNode(FactoryNode *parent, const std::string &, const std::string &,
                const NodeType&, const State &state);
    virtual ~FactoryNode();
	void addChild(FactoryNode *) override;
    void setConfirmed(const bool&);
    void setStored(const bool&);
    void setParserState(const State&);
    void setOutputState(const State&);
    id_type getMaxID() const;
	bool isConfirmed() const;
    bool isConfirmedIndirectly() const;
    bool isConfirmedFromParent() const;
    bool isStored() const;
    bool isStoredIndirectly() const;
    bool isClosedInParser() const;
    bool hasSubElements() const;
    virtual FactoryNode * getParent() const override;
    void flushXML(std::ostream&, const id_type&);
    void close(std::ostream&);
    FactoryNode * findByID(const id_type&) const;
    void print() const;
    virtual void onDestruct(void *pVoid) override;
    virtual void addListener(FactoryInput *l) override;
    virtual void removeListener(FactoryInput *l) override;
    virtual bool hasListener(FactoryInput *l) override;
	virtual void removeChild(FactoryNode *c) override;
	virtual void setParent(Node *node) override;
	void notifyChangeInGroup();
	static bool isRecusiveLink();
	static void setRecusiveLink(bool recusiveLink);
private:
    void flushIndentation(std::ostream&) const;
    void flushXMLsubnodes(std::ostream &os, const id_type &end);
};

#endif /* XMLFACTORYNODE_H_ */
