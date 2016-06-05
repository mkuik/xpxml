//
// Created by Matthijs Kuik on 8-10-2015.
//

#include <iostream>
#include "XPathCommandDeconstructor.h"

XPathCommandDeconstructor::XPathCommandDeconstructor(param_t begin, param_t end) {

	param_t logicalOperator = findLogicalOperator(begin, end);
	if(logicalOperator != end) {
		leftBegin = begin;
		leftEnd = logicalOperator;
		rightBegin = logicalOperator + 1;
		rightEnd = end;
		rightStringRangefound = leftStringRangefound = true;
	}
	else {
		param_t comparisonOperator = findComparisonOperator(begin, end);
		if(comparisonOperator != end) {
			leftBegin = begin;
			leftEnd = comparisonOperator;
			switch(type) {
			case GREATER_OR_EQUAL:
			case LESSER_OR_EQUAL:
			case UNEQUAL:
				rightBegin = comparisonOperator + 2;
				break;
			default:
				rightBegin = comparisonOperator + 1;
				break;
			}
			rightEnd = end;
			rightStringRangefound = leftStringRangefound = true;
		} else {
        }
	}

	// Find the seperator that ends the current scope
	param_t scope_end = findEndOfScopeSeparator(begin, end);

	// Handle brackets and function calls
	if(!hasRight()) {
		for(param_t it = begin; it != scope_end; ++it) {
			if(it->compare("[") == 0) {
				// Predicate located
				rightBegin = it;
				rightEnd = find(rightBegin, end, "[", "]");
				rightStringRangefound = true;
				it = rightEnd;
				rightBegin++;
				break;
			}
			else if(it->compare("(") == 0) {
				if(it != begin && (it - 1)->compare("not") == 0) {
					type = NOT;
					name = "NOT";
				}
				leftBegin = it;
				leftEnd = find(leftBegin, end, "(", ")");
				leftStringRangefound = true;
				it = leftEnd;
				leftBegin++;
			}
		}
	}

	// Get the current depth name
	if(type == NODE) extractName(begin, end);

	// Allocate the remaining scopes to left
	if(!leftStringRangefound && scope_end != end) {
		leftBegin = scope_end;
		leftEnd = end;
		leftStringRangefound = true;
	}

	rightStringRangefound = rightStringRangefound && rightBegin != rightEnd;
	leftStringRangefound = leftStringRangefound && leftBegin != leftEnd;

	std::printf("%s %i\n", name.data(), type);
	if (hasLeft()) {
		std::printf(" 0  ");
		for (param_t it = leftBegin; it != leftEnd; ++it)
			std::printf("%s", it->data());
		std::printf("\n");
	}
	if (hasRight()) {
		std::printf(" 1  ");
		for (param_t it = rightBegin; it != rightEnd; ++it)
			std::printf("%s", it->data());
		std::printf("\n");
	}
}

const std::string&XPathCommandDeconstructor::getName() const {
	return name;
}

const XPathSyntaxType&XPathCommandDeconstructor::getSyntaxType() const {
	return type;
}

const NodeScanner::Edge&XPathCommandDeconstructor::getNodePathLink() const {
	return edge;
}

const NodeType&XPathCommandDeconstructor::getNodeType() const {
	return nodetype;
}

const param_t&XPathCommandDeconstructor::getLeftBegin() const {
	return leftBegin;
}

const param_t&XPathCommandDeconstructor::getLeftEnd() const {
	return leftEnd;
}

const param_t&XPathCommandDeconstructor::getRightBegin() const {
	return rightBegin;
}

const param_t&XPathCommandDeconstructor::getRightEnd() const {
	return rightEnd;
}

const bool&XPathCommandDeconstructor::hasLeft() const {
	return leftStringRangefound;
}

const bool&XPathCommandDeconstructor::hasRight() const {
	return rightStringRangefound;
}

param_t XPathCommandDeconstructor::findEndOfScopeSeparator(const param_t& begin, param_t end) const {
	// Find the seperator that ends the current scope
	
	param_t it = begin;
	while(it != end && it->compare("/") == 0) it++;
	enum { open, predicate, bracket } mode = open;
	for(short scope = 0; it != end; ++it) {
		if(mode != bracket && it->compare("]") == 0) {
			++scope;
		}
		else if(mode != bracket && it->compare("[") == 0) {
			--scope;
			mode = predicate;
		}
		else if(mode != predicate && it->compare(")") == 0) {
			++scope;
		}
		else if(mode != predicate && it->compare("(") == 0) {
			--scope;
			mode = bracket;
		}
		else if(scope == 0) {
			mode = open;
			if(it->compare("/") == 0) {
				end = it;
				break;
			}
		}
	}
	return end;
}

param_t XPathCommandDeconstructor::find(param_t begin, param_t end, std::string left, std::string right) {
	for(short scope = 0; begin != end; ++begin) {
		if(begin->compare(right) == 0) {
			--scope;
			if(scope < 1) {
				return begin;
			}
		}
		else if(begin->compare(left) == 0) {
			++scope;
		}
	}
	return end;
}

param_t XPathCommandDeconstructor::findLogicalOperator(const param_t& begin, const param_t& end) {
	// Handle logical operators
	short scope = 0;
	for(param_t it = begin; it != end; ++it) {
		if(it->compare("]") == 0) {
			++scope;
		}
		else if(it->compare("[") == 0) {
			--scope;
		}
		else if(it->compare(")") == 0) {
			++scope;
		}
		else if(it->compare("(") == 0) {
			--scope;
		}
		else if(scope == 0) {
			if(it->compare("and") == 0) {
				type = AND;
				name = "AND";
				return it;
			}
			else if(it->compare("or") == 0) {
				type = OR;
				name = "OR";
				return it;
			}
			else if(it->compare("xor") == 0) {
				type = XOR;
				name = "XOR";
				return it;
			}
			else if(it->compare("|") == 0) {
				type = UNION;
				name = "UNION";
				return it;
			}
		}
	}
	return end;
}

param_t XPathCommandDeconstructor::findComparisonOperator(const param_t &begin, const param_t &end) {
	// Handle comparison operators
	short scope = 0;
	for(param_t it = begin; it != end; ++it) {
		if(it->compare("]") == 0) {
			--scope;
		}
		else if(it->compare("[") == 0) {
			++scope;
		}
		else if(it->compare(")") == 0) {
			--scope;
		}
		else if(it->compare("(") == 0) {
			++scope;
		}
		else if(scope == 0) {
			if(it->compare("=") == 0) {
				type = EQUAL;
				name = "EQUAL";
				return it;
			}
			else if(it->compare(">") == 0) {
				if((it + 1)->compare("=") == 0) {
					type = GREATER_OR_EQUAL;
					name = "GREATER_OR_EQUAL";
				}
				else {
					type = GREATER;
					name = "GREATER";
				}
				return it;
			}
			else if(it->compare("<") == 0) {
				if((it + 1)->compare("=") == 0) {
					type = LESSER_OR_EQUAL;
					name = "LESSER_OR_EQUAL";
				}
				else {
					type = LESSER;
					name = "LESSER";
				}
				return it;
			}
			else if(it->compare("!") == 0) {
				if((it + 1)->compare("=") == 0) {
					type = UNEQUAL;
					name = "UNEQUAL";
				}
				else {
					type = NOT;
					name = "NOT";
				}
				return it;
			}
		}
	}
	return end;
}

void XPathCommandDeconstructor::extractName(param_t begin, param_t end) {
	if((begin)->compare("(") == 0) {
		type = CONTAINER;
		name = "( ... )";
	}
	else {
		param_t it = begin;
		if((it)->compare("/") == 0) {
			++it;
			edge = NodeScanner::EDGE_DIRECT;
			if((it)->compare("/") == 0) {
				edge = NodeScanner::EDGE_ANCESTOR;
				++it;
			}
		}
		if(it->compare("@") == 0) {
			name = *(it + 1);
			nodetype = ATTRIBUTE;
			type = NODE;
		}
		else if((*it).compare("'") == 0) {
			name = *(it + 1);
			type = VALUE;
		}
		else {
			name = *it;
			try {
				convert<double>(*it);
				type = VALUE;
			}
			catch(std::exception &) {
				if(it + 1 != end && (it + 1)->compare("(") == 0) {
					if(name.compare("text") == 0) {
						type = FUNC_TEXT;
					} else if(name.compare("string-length") == 0) {
						type = FUNC_STRING_LENGTH;
					} else if (name.compare("position") == 0) {
						type = FUNC_INDEX;
					}
				}
				else {
					type = NODE;
					nodetype = ELEMENT;
				}
			}
		}
	}
}
