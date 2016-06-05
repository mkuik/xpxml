//
// Created by Matthijs Kuik on 27/10/15.
//

#ifndef XPXML2_ID_H
#define XPXML2_ID_H


typedef unsigned int id_type;

class ID {
    id_type id;
public:
    ID();
    ID(const ID&);
    virtual ~ID();
    id_type getID() const;
};


#endif //XPXML2_ID_H
