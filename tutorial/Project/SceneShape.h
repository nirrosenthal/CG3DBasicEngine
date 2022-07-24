//
// Created by ditto on 28/06/2022.
//

#ifndef ASSIGNMENT1_CPP_SCENESHAPE_H
#define ASSIGNMENT1_CPP_SCENESHAPE_H

#include <Eigen/Core>
#include "igl/opengl/glfw/Viewer.h"
#include "ObjectMover.h"
#include "LayerManager.h"

enum ShapeSource {FROM_FILE, HARD_CODED};

class SceneShape {
public:
    SceneShape(std::string shapeName, igl::opengl::glfw::Viewer::shapes shapeType,
               std::shared_ptr<ObjectMoverSplit> moverr, std::shared_ptr<Layer> layer, int *indexes);
    SceneShape(std::string shapeName, std::string file,
               std::shared_ptr<ObjectMoverSplit> moverr, std::shared_ptr<Layer> layer, int *indexes);
    std::shared_ptr<Layer> getLayer();
    void changeLayer(std::shared_ptr<Layer> layer);
    int getIndex();
    std::string name;
    bool isDrawn(float time);
    Eigen::Vector3f getPosition(float time);
    Eigen::Vector3f getlastDrawnPosition(int id);
    void setlastDrawnPosition(Eigen::Vector3f pos, int id);
    void addMover(std::shared_ptr<ObjectMover> mover);
    int getParent();
    void setParent(int newParent);
    void addChild(int child);
    void removeChild(int child);
    std::vector<int> getChildren();
    float getEndTime();
    int material = -1;
    int shader = -1;
    Eigen::Vector3f dimensions;
    bool isScaledToZero;
    int index;
    igl::opengl::glfw::Viewer::shapes type;
    std::shared_ptr<ObjectMoverSplit> mover;
    ShapeSource getSource() {return source;};
    std::string getFile() {return file;};
    float GetNormalizedScale();
    float GetScale();
    void Rescale(float newSize);
    bool NeedsRescale(int id);
    void MarkAsRescaled(int id);
    std::vector<int> GetIds();

private:
    std::shared_ptr<Layer> layer;
    Eigen::Vector3f lastDrawnPositions[4];
    int ids[4];
    int parent;
    std::vector<int> children;
    ShapeSource source;
    std::string file;
    float sizePercent;
    float previousSize;
    std::vector<int> idsToRescale;
};


#endif //ASSIGNMENT1_CPP_SCENESHAPE_H
