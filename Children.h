//
// Created by Matthijs on 2-7-2015.
//

#ifndef XPXML2_CHILDREN_H
#define XPXML2_CHILDREN_H


#include <list>

template <class T> class Children {
protected:
    std::list<T*> children;
public:
    Children() {}
    ~Children() {
    }

	virtual const std::list<T*>& getChildren() const {
		return children;
	}
    virtual void addChild(T* c) {
        children.push_back(c);
    }

    virtual void removeChild(T* c) {
        children.remove(c);
    }
    void clearChildren() {
        children.clear();
    }
	bool isParent() const {
		return !children.empty();
	}
	T* getLastChild() const {
		return children.back();
	}
};



#endif //XPXML2_CHILDREN_H
