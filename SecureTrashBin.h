//
// Created by Matthijs Kuik on 09/09/15.
//

#ifndef XPXML2_SECURETRASHBIN_H
#define XPXML2_SECURETRASHBIN_H

#include <set>

template <class T>
class SecureTrashBin {
    std::set<T*> trash;
public:
    ~SecureTrashBin() {

    }
    void emptyTrash(T *exception) {
        for (auto it = trash.begin(); it != trash.end();) {
            T* item = *it;
            if (item == exception) {
                ++it;
            } else {
                it = trash.erase(it);
                removeFromTrash(item);
            }
        }
    }
    void emptyTrash() {
        for (auto it = trash.begin(); it != trash.end();) {
            T* item = *it;
            it = trash.erase(it);
            removeFromTrash(item);
        }
    }
    virtual void addToTrash(T* item) {
        trash.insert(item);
    }
    virtual void removeFromTrash(T* item) {

    }
};


#endif //XPXML2_SECURETRASHBIN_H