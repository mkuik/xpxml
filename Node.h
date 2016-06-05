/*
 * xNode.h
 *
 *  Created on: Nov 17, 2014
 *      Author: matthijs
 */

#ifndef NODE_H_
#define NODE_H_

#include <string>
#include <iostream>
#include "Directory.h"
#include "Children.h"
#include "NodeType.h"
#include "Id.h"
#include "Value.h"
#include "Namespace.h"
#include "DestructorAdapter.h"

class Node :
        public Directory,
        public ID,
        public Value,
        public Namespace,
        public DestructorAdapter,
        public Children<Node> {
    NodeType type;
    std::string xml_namespace;
    unsigned short link = 0;
public:
    static id_type nodeCount;
    Node(Node * = 0, NodeType = VIRTUAL);
    Node(const std::string &, const std::string &, Node * = 0,
         NodeType = VIRTUAL);
    Node(const Node &);
    virtual ~Node();
    NodeType getType() const;
    Node *getNonVirtualParent() const;
    unsigned int getDepth() const override;
    id_type getMaxID() const;
    virtual Node *getParent() const override;
    std::string getPath() const override;
    void setType(const NodeType &);
    virtual void setParent(Node *);
    virtual std::string toString() const override;
    virtual std::string toStringOwners() const;
    virtual void addListener(DestructorListener *l) override;
    void notifyLink();
    void notifyUnlink();
    Node * findByID(const id_type&) const;
    unsigned short getLink() const;
};


#endif /* NODE_H_ */