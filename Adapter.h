/*
 * DataCommunication.h
 *
 *  Created on: Feb 25, 2015
 *      Author: matthijs
 */

#ifndef CONTACT_H
#define CONTACT_H

#include <list>
#include <cstdio>
#include <iostream>
#include <sys/timeb.h>

template<class T>
class Adapter {
protected:
	std::list<T*> listeners;
	T * activated = 0, * remove = 0;
public:
	Adapter() {
	}
    virtual ~Adapter() {
        listeners.clear();
    }
	virtual void addListener(T* l) {
        listeners.push_front(l);
	}
    virtual void removeListener(T* l) {
		if (l == activated) remove = l;
		else listeners.remove(l);
	}

    virtual bool hasListener(T* l) {
        for (auto lis : listeners) {
            if (lis == l) return true;
        }
        return false;
    }
protected:
	void start() { }
	void stop() {
		activated = 0;
	}
};

#endif /* H_ */
