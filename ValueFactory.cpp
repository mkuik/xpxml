/*
 * XPathToText.cpp
 *
 *  Created on: Jun 14, 2015
 *      Author: matthijs
 */

#include <sstream>
#include "ValueFactory.h"

ValueFactory::ValueFactory(const std::string& xpath) :
		Factory(xpath) {

}

ValueFactory::~ValueFactory() {
}

void ValueFactory::onNewXPathMatch(Node* data) {
	Factory::onNewXPathMatch(data);
	std::stringstream stream;
	stream << data->getValue() << std::endl;
	notifyOutput(stream);
}

size_t ValueFactory::size() const {
	return 0;
}
