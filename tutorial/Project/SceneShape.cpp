//
// Created by ditto on 28/06/2022.
//

#include "SceneShape.h"

SceneShape::SceneShape(std::string shapeName, igl::opengl::glfw::Viewer::shapes shapeType,
                       std::shared_ptr<ObjectMoverSplit> moverr, std::shared_ptr<Layer> layer, int *indexes) :
                       name(shapeName), type(shapeType),layer(layer),
                       dimensions(Eigen::Vector3f(1,1,1)), isScaledToZero(false),
                       mover(moverr)
                       {
                            for(int i=0; i<4; i++) {
                                lastDrawnPositions[i] = Eigen::Vector3f(0,0,0);
                                ids[i] = indexes[i];
                            }
                       }

std::shared_ptr<Layer> SceneShape::getLayer() {
    return layer;
}
void SceneShape::changeLayer(std::shared_ptr<Layer> layer) {
    this->layer = layer;
}
int SceneShape::getIndex() {
    return index;
}


bool SceneShape::isDrawn(float time) {
    return mover->isDrawnAt(time) && !layer->isHidden();
}

Eigen::Vector3f SceneShape::getPosition(float time) {
    return mover->getPosition(time);
}

Eigen::Vector3f SceneShape::getlastDrawnPosition(int id) {
    for(int i=0; i<4; i++) {
        if (id == ids[i]) {
            return lastDrawnPositions[i];
        }
    }
}

void SceneShape::setlastDrawnPosition(Eigen::Vector3f pos, int id) {
    for(int i=0; i<4; i++) {
        if(id == ids[i]) {
            lastDrawnPositions[i] = pos;
            break;
        }
    }
}

void SceneShape::addMover(std::shared_ptr<ObjectMover> mover) {
    this->mover->addMover(mover);

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
    return mover->getEndTime();
}


