//
// Created by Matthijs Kuik on 27/10/15.
//

#ifndef XPXML2_VALUE_H
#define XPXML2_VALUE_H

#include <string>

class Value {
    std::string value;
public:
    Value(const std::string& = "");
    Value(const Value&);
    ~Value();
    const std::string& getValue() const;
    virtual void setValue(const std::string&);
    void addToValue(const int&);
};


#endif //XPXML2_VALUE_H
