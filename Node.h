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
#include "NodeAdapter.h"

class Node :
        public Directory,
        public ID,
        public Value,
        public Namespace,
        public NodeAdapter {
    NodeType type;
    std::string xml_namespace;
    unsigned short link = 0;
    bool open = true;
	bool match = false;
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

    virtual Node *getParent() const override;

    std::string getPath() const override;

    void setType(const NodeType &);

    virtual void setParent(Node *);

    virtual std::string toString() const override;

    void notifyLink();

    void notifyUnlink();

    unsigned short getLink() const;

    bool isOpen() const;

    void setOpen(bool open);

	bool isMatch() const;

	void setMatch(bool match);
};


#endif /* NODE_H_ */