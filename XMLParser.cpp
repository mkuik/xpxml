//
// Created by Matthijs Kuik on 6-10-2015.
//
#include "XMLParser.h"
#include "XMLFactory.h"
#include "ValueFactory.h"
#include "Globals.h"
#include <sstream>
#include <sys/timeb.h>
#include <ctime>

XMLParser::XMLParser(std::ostream &out) : output(out), parser(new SaxParser()) {
}

XMLParser::~XMLParser() {
	delete parser;
	if (factory) delete factory;
}

int XMLParser::parse(const char *file, const char *xpath, const Method &method) {
    starttime = getMilliCount();
	
	switch(method) {
        case TO_XML:
            factory = new XMLFactory(xpath);
            break;
        case GET_VALUES:
            factory = new ValueFactory(xpath);
            break;
    }

    factory->setAdapter(parser);
	parser->addListener(this);
    factory->addListener(this);
    return parser->parse(file);
}

void XMLParser::stop() {
    parser->abort();
}

void XMLParser::setNodeIDLimit(const id_type &id) {
    maxID = id;
}

void XMLParser::onEndOfElement(Node *node) {
    if (maxID != 0 && maxID <= node->getID())
        stop();
}

double XMLParser::getProgress() const {
    return parser->percentage();
}

void XMLParser::writeResult(std::stringstream &stream) {
    output << stream.str();
}

time_t XMLParser::getMilliCount() const {
    timeb tb;
    ftime(&tb);
    return tb.millitm + (tb.time & 0xfffff) * 1000;
}

void XMLParser::onNewNode(Node *node) {
    if (node->getID() % 1000 == 0) doStatusReport();
}

void XMLParser::onEndOfFile() {
	doStatusReport();
	std::printf("\n");
}

void XMLParser::doStatusReport() {
    if(&output != &std::cout) {
        time_t ms = (getMilliCount() - starttime);
        auto mb = parser->getPosition() / 1000.0 / 1000.0;
        double mbps = mb / (ms / 1000.0);
        double progress = getProgress();
        time_t eta = ms / progress - ms;

        auto hours = eta / 3600000;
        auto minutes = (eta / 60000) % 60;
        auto seconds = (eta / 1000) % 60;

        std::stringstream time;
        if (hours > 0) time << hours << "h ";
        if (minutes > 0 || hours > 0) time << minutes << "m ";
        time << seconds << "s";
//        time << ms % 1000 << "";

        std::printf("Parsing... %i%%  ETA:%s  %.1fmbps  %s  %li matches  \r", 
			((int) (progress * 100 + 0.5)), time.str().data(), mbps, 
			factory->toString().data(), hits);
    }
}

void XMLParser::newHit(Node *node) {
    WriteInput::newHit(node);
    ++hits;
}
