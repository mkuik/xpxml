//
// Created by Matthijs on 8-11-2015.
//

#ifndef XPXML2_NAMESPACE_H
#define XPXML2_NAMESPACE_H

#include <string>

class Namespace {
	std::string name;
public:
	Namespace(const std::string& = "");
	Namespace(const Namespace&);
	virtual ~Namespace();
	const std::string& getNamespace() const;
	virtual void setNamespace(const std::string&);
	void addToNamespace(const int&);
};


#endif //XPXML2_NAMESPACE_H