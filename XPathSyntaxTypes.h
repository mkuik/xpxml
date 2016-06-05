//
// Created by Matthijs Kuik on 8-10-2015.
//

#ifndef XPXML2_XPATHSYNTAXTYPES_H
#define XPXML2_XPATHSYNTAXTYPES_H

enum XPathSyntaxType {
    NODE,
    GREATER,
    LESSER,
    GREATER_OR_EQUAL,
    LESSER_OR_EQUAL,
    EQUAL,
    UNEQUAL,
    AND,
    OR,
    XOR,
    NOT,
    UNION,
    VALUE,
    CONTAINER,
    FUNC_TEXT,
    FUNC_STRING_LENGTH,
	FUNC_INDEX,
	INDEX
};

#endif //XPXML2_XPATHSYNTAXTYPES_H
