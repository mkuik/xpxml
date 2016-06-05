//
// Created by Matthijs on 2-7-2015.
//

#ifndef XPXML2_PARENT_H
#define XPXML2_PARENT_H


template <class T> class Parent {
    T* parent;
public:
    Parent(T* p = 0) : parent(p) {}
    Parent(const Parent& p) : parent(p.parent) {}
    virtual ~Parent() { }
    virtual T* getParent() const {
        return parent;
    }
    T* getTopParent() const {
		if (hasParent()) return getParent()->getTopParent();
        else return (T*)this;
    }

    virtual void setParent(T* p) {
        parent = p;
    }
    virtual unsigned int getDepth() const {
        if (getParent()) return getParent()->getDepth() + 1;
        else return 0;
    }
    bool hasParent() const {
        return parent != 0;
    }
    void clearParent() {
        parent = 0;
    }
};


#endif //XPXML2_PARENT_H
