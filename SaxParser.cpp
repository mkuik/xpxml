/*
 * xSaxParser.cpp
 *
 *  Created on: Nov 17, 2014
 *      Author: matthijs
 */

#include <stdexcept>
#include "SaxParser.h"

SaxParser::SaxParser() : Name("sax parser") {
}

SaxParser::~SaxParser() {
	if(node)
		delete node->getTopParent();
}

int SaxParser::parse(const char * filename) {
	enum { NEW_ELEMENT, END_ELEMENT, ELEMENT_VALUE, NEW_ATTRIBUTE, ATTRIBUTE_VALUE, SPACING, COMMENT } state = SPACING;
	
	std::ifstream file;
    file.open(filename, std::ios::binary);

    if (!file) {
        std::string error = "File not found: ";
        throw std::runtime_error(error + filename);
    }

	file.seekg(0, file.end);
	filesize = file.tellg();
	file.seekg(0, file.beg);

	std::streambuf * pbuf = file.rdbuf();
	position = 0;
	Node * temp = 0;

	while(!stop && pbuf->sgetc() != EOF) {
		position++;
		int key = pbuf->sbumpc();

//		std::cout << (char)key << "";

		switch(state) {
		case NEW_ELEMENT:
			switch(key) {
			case ('>') :
				newElement(node);
				state = ELEMENT_VALUE;
				break;
			case ('/') :
				temp = node;
				node = node->getParent();
				delete temp;
				state = END_ELEMENT;
				break;
			case (' ') :
				newElement(node);
				state = NEW_ATTRIBUTE;
				node = new Node(node, ATTRIBUTE);
				break;
			case ('?') :
				if(node->getName().empty()) {
					node->setType(INSTRUCTION);
				}
				break;
			case (':') :
				node->setNamespace(node->getName());
				node->setName("");
				break;
			default :
				node->addToName(key);
				if(node->getName().size() == 3 && node->getName().compare("!--") == 0) {
					state = COMMENT;
					node->setType(VIRTUAL);
				}
				break;
			}
			break;
		case END_ELEMENT:
			switch(key) {
			case ('>') :
				temp = node;
				node = node->getParent();
				endElement(temp);
				state = SPACING;
				break;
			default:
				break;
			}
			break;
		case ELEMENT_VALUE:
			switch(key) {
			case ('<') :
				state = NEW_ELEMENT;
				node = new Node(node, ELEMENT);
				break;
			default:
				node->addToValue(key);
				break;
			}
			break;
		case NEW_ATTRIBUTE:
			switch(key) {
			case ('>') :
				temp = node;
				node = node->getParent();
				delete temp;
				state = ELEMENT_VALUE;
				break;
			case ('?') :
			case ('/') :
				temp = node;
				node = node->getParent();
				delete temp;
				state = END_ELEMENT;
				break;
			case ('=') :
				state = ATTRIBUTE_VALUE;
				break;
			default:
				node->addToName(key);
				break;
			}
			break;
		case ATTRIBUTE_VALUE:
			switch(key) {
			case ('>') :
				temp = node;
				node = node->getParent();
				newAttribute(temp);
				state = ELEMENT_VALUE;
				break;
			case ('?') :
			case ('/') :
				temp = node;
				node = node->getParent();
				newAttribute(temp);
				state = END_ELEMENT;
				break;
			case ('"') :
				while(pbuf->sgetc() != EOF) {
					position++;
					auto key = pbuf->sbumpc();
//					std::cout << (char) key << "";
					if(key == '"') break;
					node->addToValue(key);
				}
				break;
			case ('\n') :
				break;
			case (' ') :
				temp = node;
				node = node->getParent();
				newAttribute(temp);
				node = new Node(node, ATTRIBUTE);
				state = NEW_ATTRIBUTE;
				break;
			default:
				node->addToValue(key);
				break;
			}
			break;
		case SPACING:
			switch(key) {
			case ('<') :
				state = NEW_ELEMENT;
				node = new Node(node, ELEMENT);
				break;
			default:
				break;
			}
			break;
		case COMMENT:
			switch(key) {
			case ('>') :
				if(node->getValue().substr(node->getValue().size() - 2).compare("--") == 0) {
					node->setValue(node->getValue().substr(0, node->getValue().size() - 2));
					temp = node;
					node = node->getParent();
					newComment(temp);
					state = SPACING;
				}
				else {
					node->addToValue(key);
				}
				break;
			default:
				node->addToValue(key);
				break;
			}
			break;
		}
	}

	file.close();
	return 0;
}

void SaxParser::abort() {
	stop = true;
	std::printf("\n!ABORT PARSER\n\n");
}

double SaxParser::percentage() const {
	return position / (double) filesize;
}

unsigned long SaxParser::getPosition() const {
	return position;
}

void SaxParser::newElement(Node* node) {
	std::cout << "\nELE " << node->toString() << std::endl;
	activated = node;
	notifyNewElement(node);
	notifyNewNode(node);
}

void SaxParser::endElement(Node* node) {
	std::cout << "\nEOF " << node->toString() << std::endl;
	activated = node;
	node->setOpen(false);
	notifyEndOfElement(node);

	if (node->getLink() == 0) {
		delete node;
	} 
}

void SaxParser::newAttribute(Node* node) {
	std::cout << "\nATT " << node->toString() << std::endl;
	activated = node;
	notifyNewAttribute(node);
	notifyNewNode(node);

	if (node->getLink() == 0) {
		delete node;
	}
}

void SaxParser::newComment(Node * node) {
	std::cout << "\nCOM " << node->toString() << std::endl;
	activated = node;
	notifyComment(node);

	if (node->getLink() == 0) {
		delete node;
	} 
}