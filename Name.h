//
// Created by Matthijs on 2-7-2015.
//

#ifndef XPXML2_NAME_H
#define XPXML2_NAME_H

#include <string>

class Name {
    std::string name;
public:
    Name(const std::string& = "");
    Name(const Name&);
    virtual ~Name();
    const std::string& getName() const;
    virtual void setName(const std::string&);
	void addToName(const int&);
};


#endif //XPXML2_NAME_H