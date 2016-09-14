/*
 * xPath.h
 *
 *  Created on: Dec 9, 2014
 *      Author: matthijs
 */

#ifndef UNISONFILTER_H_
#define UNISONFILTER_H_

#include <vector>
#include <string>

#include "XPathScanner.h"
#include "SaxParser.h"
#include "WriteContact.h"

class Factory : public XPathScannerListener, public WriteOutput, public Name {
protected:
	std::vector<XPathScanner *> structures;
public:
	Factory(const std::string&);
	~Factory();
	void deleteXPathScanners();
	virtual void setAdapter(SaxParserAdapter *);
	void moveStream(std::stringstream&);
	virtual size_t size() const = 0;
	size_t getXPathScannerSize() const;
	virtual double getEfficiency() const;
	virtual std::string toString() const;
	virtual void onNewXPathMatch(Node *node) override;
};

#endif /* UNISONFILTER_H_ */