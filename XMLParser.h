//
// Created by Matthijs Kuik on 6-10-2015.
//

#ifndef XPXML2_XMLPARSER_H
#define XPXML2_XMLPARSER_H


#include "SaxParser.h"
#include "Factory.h"
#include "FactoryNodeAdapter.h"

class XMLParser: public SaxParserListener, public WriteInput, public FactoryListener {
public:
	enum Method { GET_VALUES, TO_XML };
private:
    SaxParser * const parser;
    Factory * factory = 0;
    std::ostream& output;
    id_type maxID = 0;
	double progress = -1;
	long hits = 0;
	time_t starttime;
public:
    XMLParser(std::ostream&);
    ~XMLParser();
    int parse(const char*, const char*, const Method&);
    void stop();
    void setNodeIDLimit(const id_type&);
    virtual void onEndOfElement(Node *) override;
	virtual void onNewNode(Node *) override;
	virtual void onEndOfFile() override;
	virtual void writeResult(std::stringstream&) override;
	double getProgress() const;
	time_t getMilliCount() const;
    void doStatusReport();

	virtual void newHit(Node *node) override;
};


#endif //XPXML2_XMLPARSER_H
