//
// Created by ditto on 28/06/2022.
//

#include "SceneShape.h"

SceneShape::SceneShape(std::string shapeName, igl::opengl::glfw::Viewer::shapes shapeType,
                       std::shared_ptr<ObjectMover> mover, std::shared_ptr<Layer3> layer, int index) :
                       name(shapeName), type(shapeType), mover(ObjectMoverSplit(mover)),
                       layer(layer), index(index) {}

std::shared_ptr<Layer3> SceneShape::getLayer() {
    return layer;
}
void SceneShape::changeLayer(std::shared_ptr<Layer3> layer) {
    this->layer = layer;
}
int SceneShape::getIndex() {
    return index;
}


bool SceneShape::isDrawn(float time) {
    return mover.isDrawnAt(time);
}

Eigen::Vector3f SceneShape::getPosition(float time) {
    return mover.getPosition(time);
}

Eigen::Vector3f SceneShape::getlastDrawnPosition() {
    return lastDrawnPosition;
}

void SceneShape::setlastDrawnPosition(Eigen::Vector3f pos) {
    lastDrawnPosition = pos;
}

void SceneShape::addMover(std::shared_ptr<ObjectMover> mover) {
    this->mover.addMover(mover);

}

int SceneShape::getParent() {
    return parent;
}

void SceneShape::setParent(int newParent) {
    parent = newParent;
}

std::vector<int> SceneShape::getChildren() {
    return children;
}

void SceneShape::addChild(int child) {
    children.push_back(child);
}

void SceneShape::removeChild(int removedChild) {
    std::vector<int> newChildren;
    for(int child : children) {
        if(child != removedChild)
            newChildren.push_back(child);
    }
    children = newChildren;
}

float SceneShape::getEndTime() {
    return mover.getEndTime();
}


