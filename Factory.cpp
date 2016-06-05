/*
 * xPath.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: matthijs
 */

#include "Factory.h"
#include "Globals.h"
#include <sstream>
#include <string>

Factory::Factory(const std::string &xpath) : Name("factory") {


    std::vector<std::string> xpath_split_by_unison{""};
    short scope = 0;
    for (char c : xpath) {
        switch (c) {
            case '(':
            case '[':
                ++scope;
                xpath_split_by_unison.back() += c;
                break;
            case ')':
            case ']':
                --scope;
                xpath_split_by_unison.back() += c;
                break;
            case '|':
                if (scope == 0)
                    xpath_split_by_unison.push_back("");
                else
                    throw "unison found within scope of brackets";
                break;
            case ' ':
                if (!xpath_split_by_unison.back().empty())
                    xpath_split_by_unison.back() += c;
                break;
            default:
                xpath_split_by_unison.back() += c;
                break;
        }
    }

    for (std::string xpath_location : xpath_split_by_unison) {
        XPathScanner *path = new XPathScanner(xpath_location);
        path->addListener(this);
        structures.push_back(path);
    }
}

Factory::~Factory() {
    Globals::DESTRUCTING = true;
    for (auto it : structures) {
        delete it;
    }
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

void Factory::onNewXPathMatch(Node *node) {
    XPathScannerListener::onNewXPathMatch(node);
    notifyHit(node);
}
