/*
 * xNodeWrapper.h
 *
 *  Created on: Nov 19, 2014
 *      Author: matthijs
 */

#ifndef SAXINTERPRETER_H_
#define SAXINTERPRETER_H_

#include "Node.h"
#include <list>

#include "NodeScannerAdapter.h"
#include "SaxParserAdapter.h"


class NodeScanner : public Node,
				   public Children<NodeScanner>,
                   public SaxParserListener,
                   public NodeScannerAdapter {
public:
    enum Edge {
        EDGE_DIRECT, EDGE_ANCESTOR, EDGE_ROOT
    };
private:
	Edge connection;
	bool wildcard = false;
public:
	NodeScanner(NodeScanner *, NodeType, Edge);
	~NodeScanner();
    virtual NodeScanner * getParent() const override;
    void setParent(NodeScanner *);
	virtual void setName(const std::string&) override;
	Edge getConnectionType() const;
	void onNewNode(Node *node) override;
	std::string toString() const override;
	bool cmpDir(Node *, const NodeScanner * = 0) const;
	bool isWildcard() const;
    virtual void addListener(NodeScannerListener *) override;
    virtual void removeListener(NodeScannerListener *) override;
    virtual bool hasListener(NodeScannerListener *) override;
};

void merge(NodeScanner *, NodeScanner *);

#endif /* SAXINTERPRETER_H_ */
