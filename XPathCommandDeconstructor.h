//
// Created by Matthijs Kuik on 8-10-2015.
//

#ifndef XPXML2_XPATHSYNTAXFACTORY_H
#define XPXML2_XPATHSYNTAXFACTORY_H

#include "XPathSyntaxTypes.h"
#include "NodeType.h"
#include "NodeScanner.h"
#include "Xpath.h"

#include <string>
#include <vector>

class XPathCommandDeconstructor {
private:
    XPathSyntaxType type = NODE;
	Xpath left;
	Xpath right;
    NodeScanner::Edge edge = NodeScanner::EDGE_DIRECT;
    NodeType nodetype = VIRTUAL;
    std::string name;
public:
	XPathCommandDeconstructor(const Xpath&);
	~XPathCommandDeconstructor();
    const std::string& getName() const;
    const XPathSyntaxType& getSyntaxType() const;
    const NodeScanner::Edge& getNodePathLink() const;
    const NodeType& getNodeType() const;
	Xpath getLeft() const;
	Xpath getRight() const;
	bool hasLeft() const;
	bool hasRight() const;
private:
    void extractName(param_t, param_t);
    param_t findLogicalOperator(const param_t&, const param_t&);
    param_t findComparisonOperator(const param_t&, const param_t&);
    param_t findEndOfScopeSeparator(const param_t&, const param_t&) const;
    param_t find(const param_t&, const param_t&, std::string, std::string);
};


#endif //XPXML2_XPATHSYNTAXFACTORY_H
