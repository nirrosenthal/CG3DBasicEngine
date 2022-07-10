//
// Created by ditto on 10/07/2022.
//

#ifndef ASSIGNMENT1_CPP_OBJECTMOVERFORGUI_H
#define ASSIGNMENT1_CPP_OBJECTMOVERFORGUI_H


#include <memory>
#include "ObjectMover.h"

class ObjectMoverForGui {
public:
    ObjectMoverForGui(std::shared_ptr<ObjectMover> mover);
    ObjectMoverForGui();
    std::vector<float *> points;
    MoverType moverType;
    float duration;
    ~ObjectMoverForGui();
};


#endif //ASSIGNMENT1_CPP_OBJECTMOVERFORGUI_H
