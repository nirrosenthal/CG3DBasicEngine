//
// Created by ditto on 10/07/2022.
//

#include "ObjectMoverForGui.h"

ObjectMoverForGui::ObjectMoverForGui(std::shared_ptr<ObjectMover> mover):
  duration(mover->getEndTime() - mover->getStartTime()), moverType(mover->getTag()) {
    if(moverType == CONSTANT) {
        auto constMover = std::dynamic_pointer_cast<ObjectMoverConstant>(mover);
        float *point = new float[3]{constMover->position[0], constMover->position[1], constMover->position[2]};
        points.push_back(point);
    } else if(moverType == BEZIER) {
        auto bezMover = std::dynamic_pointer_cast<ObjectMoverBezier>(mover);
        for(auto vec : bezMover->points) {
            float *point = new float[3]{vec[0], vec[1], vec[2]};
            points.push_back(point);
        }
    } else {
        throw std::logic_error("Cannot cast a split mover to gui mover");
    }
}

ObjectMoverForGui::ObjectMoverForGui(): duration(0), moverType(CONSTANT) {}

ObjectMoverForGui::~ObjectMoverForGui() {
    for(auto p : points)
        delete[] p;
    points.clear();
}
