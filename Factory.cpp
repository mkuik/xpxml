/*
 * xPath.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: matthijs
 */

#include "Factory.h"
#include "Globals.h"
#include "Xpath.h"
#include <sstream>
#include <string>

Factory::Factory(const std::string &xpath) : Name("factory") {
    for (const Xpath& x : XPathCollection(xpath)) {
        XPathScanner *path = new XPathScanner(x);
        path->addListener(this);
        structures.push_back(path);
    }
}

Factory::~Factory() {
    Globals::DESTRUCTING = true;
    deleteXPathScanners();
}


void Factory::setAdapter(SaxParserAdapter *parser) {
    for (XPathScanner *tree : structures) {
        tree->setAdapter(parser);
    }
}

void Factory::moveStream(std::stringstream& stream) {
    stream.seekp(0, std::ios::end);
    std::stringstream::pos_type offset = stream.tellp();
    if ((int)offset != 0) {
        stream.seekp(0, std::ios::beg);
        notifyOutput(stream);
    }
}

size_t Factory::getXPathScannerSize() const {
    size_t n = 0;
    for (XPathScanner * scanner : structures) {
        n += scanner->getSize();
    }
    return n;
}

double Factory::getEfficiency() const {
    return 1;
}

std::string Factory::toString() const {
	std::stringstream s;
	s << getEfficiency() << "%";
	return s.str();
}

void Factory::onNewXPathMatch(Node *node) {
    XPathScannerListener::onNewXPathMatch(node);
    notifyHit(node);
}

void Factory::deleteXPathScanners() {
    for (auto it = structures.begin(); it != structures.end(); ) {
        XPathScanner * scanner = *it;
        it = structures.erase(it);
        delete scanner;
    }
}


