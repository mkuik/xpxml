//
// Created by Matthijs Kuik on 8-10-2015.
//

#ifndef XPXML2_XPATHSYNTAXFACTORY_H
#define XPXML2_XPATHSYNTAXFACTORY_H

#include "XPathSyntaxTypes.h"
#include "tools.h"
#include "NodeType.h"
#include "NodeScanner.h"

#include <string>
#include <vector>

class XPathCommandDeconstructor {
private:
    XPathSyntaxType type = NODE;
    param_t leftBegin, leftEnd, rightBegin, rightEnd;
    bool leftStringRangefound = false;
    bool rightStringRangefound = false;
    NodeScanner::Edge edge = NodeScanner::EDGE_DIRECT;
    NodeType nodetype = VIRTUAL;
    std::string name;
public:
    XPathCommandDeconstructor(param_t, param_t);
    const std::string& getName() const;
    const XPathSyntaxType& getSyntaxType() const;
    const NodeScanner::Edge& getNodePathLink() const;
    const NodeType& getNodeType() const;
    const bool& hasLeft() const;
    const bool& hasRight() const;
    const param_t& getLeftBegin() const;
    const param_t& getLeftEnd() const;
    const param_t& getRightBegin() const;
    const param_t& getRightEnd() const;
private:
    void extractName(param_t, param_t);
    param_t findLogicalOperator(const param_t&, const param_t&);
    param_t findComparisonOperator(const param_t&, const param_t&);
    param_t findEndOfScopeSeparator(const param_t&, param_t) const;
    param_t find(param_t, param_t, std::string, std::string);
};


#endif //XPXML2_XPATHSYNTAXFACTORY_H
