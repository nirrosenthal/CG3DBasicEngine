//
// Created by nirrose@wincs.cs.bgu.ac.il on 24/07/2022.
//

#include "SceneCamera.h"

SceneCamera::SceneCamera() {
    angle = 0.0f;
    relationWH = 0;
    near = 1.0f;
    far = 120.0f;
}

SceneCamera::SceneCamera(std::string _name, int _id, float _angle, float _relationWH, float _near, float _far,
                         std::shared_ptr<ObjectMoverSplit> _mover):
        name(_name), id(_id), angle(_angle), relationWH(_relationWH), near(_near), far(_far), mover(_mover) {}

int SceneCamera::GetId() {
    return id;
}

std::string SceneCamera::GetName() {
    return name;
}