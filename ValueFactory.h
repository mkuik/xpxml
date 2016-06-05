/*
 * XPathToText.h
 *
 *  Created on: Jun 14, 2015
 *      Author: matthijs
 */

#ifndef XPATHTOTEXT_H_
#define XPATHTOTEXT_H_

#include "Factory.h"

class ValueFactory : public Factory {
public:
	ValueFactory(const std::string&);
	~ValueFactory();
	void onNewXPathMatch(Node*);
	size_t size() const;
};

#endif /* XPATHTOTEXT_H_ */
