/*
 * xSaxParser.h
 *
 *  Created on: Nov 17, 2014
 *      Author: matthijs
 */

#ifndef XSAXPARSER_H_
#define XSAXPARSER_H_

#include <fstream>
#include <string.h>
#include "SaxParserAdapter.h"
#include "Node.h"

class SaxParser : public SaxParserAdapter, public Name {
	bool stop = false;
	Node * node = 0, *activated = 0;
	std::streampos filesize;
	unsigned long position;
public:
	SaxParser();
	~SaxParser();
	int parse(const char*);
	void abort();
	double percentage() const;
	unsigned long getPosition() const;
private:
	void newElement(Node*);
	void endElement(Node*);
	void newAttribute(Node*);
	void newComment(Node*);
};
#endif /* XSAXPARSER_H_ */
