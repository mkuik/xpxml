//
// Created by Matthijs Kuik on 6-10-2015.
//

#ifndef XPXML2_OBJECTTYPECOUNTER_H
#define XPXML2_OBJECTTYPECOUNTER_H


class ObjectTypeCounter {
public:
    static unsigned int count;
    ObjectTypeCounter();
    ~ObjectTypeCounter();
    unsigned int getTypeCount() const;
};


#endif //XPXML2_OBJECTTYPECOUNTER_H