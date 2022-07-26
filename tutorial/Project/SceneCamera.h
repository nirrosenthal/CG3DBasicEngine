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
    SceneCamera(std::string _name, int _id, Eigen::Vector3d _pos, std::shared_ptr<ObjectMoverSplit> _mover);
    std::shared_ptr<ObjectMoverSplit> mover;
    std::string name;
    int id = 4;
    Eigen::Vector3d pos;
    int GetId();
    std::string GetName();
    Eigen::Vector3d  GetPosition();
private:

};

#endif //ENIGEREWORK_SCENECAMERA_H
