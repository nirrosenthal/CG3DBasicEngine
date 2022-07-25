//
// Created by nirrose@wincs.cs.bgu.ac.il on 24/07/2022.
//

#ifndef ENIGEREWORK_SCENECAMERA_H
#define ENIGEREWORK_SCENECAMERA_H

#include <Eigen/Core>
#include "igl/opengl/glfw/Viewer.h"
#include "ObjectMover.h"

class SceneCamera {
public:
    SceneCamera(std::string _name, int _id, float _angle, float _relationWH, float _near, float _far, std::shared_ptr<ObjectMoverSplit> _mover);
    // constructor with default values
    SceneCamera();
    std::shared_ptr<ObjectMoverSplit> mover;
    std::string name;
    int id;
    float angle;
    float relationWH;
    float near;
    float far;
    int GetId();
    std::string GetName();
private:

};

#endif //ENIGEREWORK_SCENECAMERA_H
