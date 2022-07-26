//
// Created by nirrose@wincs.cs.bgu.ac.il on 24/07/2022.
//

#include "SceneCamera.h"

SceneCamera::SceneCamera(std::string _name, int _id, Eigen::Vector3d _pos,
                         std::shared_ptr<ObjectMoverSplit> _mover):
        name(_name), id(_id), pos(_pos), mover(_mover) {}

Eigen::Vector3d SceneCamera::GetPosition() {
    return pos;
}

int SceneCamera::GetId() {
    return id;
}

std::string SceneCamera::GetName() {
    return name;
}