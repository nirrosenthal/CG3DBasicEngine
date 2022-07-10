//
// Created by ditto on 28/06/2022.
//

#ifndef ASSIGNMENT1_CPP_OBJECTMOVER_H
#define ASSIGNMENT1_CPP_OBJECTMOVER_H
#include <Eigen/Core>
#include <vector>
#include <memory>
enum MoverType {SPLIT, CONSTANT, BEZIER};

class ObjectMover {
public:
    virtual Eigen::Vector3f getPosition(float time)=0;
    virtual float getStartTime()=0;
    virtual float getEndTime()=0;
    virtual MoverType getTag()=0;
    virtual void shift(Eigen::Vector3f shiftValue)=0;
    bool isDrawnAt(float time);
    virtual std::shared_ptr<ObjectMover> clone()=0;
};

class ObjectMoverSplit: public ObjectMover {
public:
    ObjectMoverSplit(std::shared_ptr<ObjectMover> firstMover, std::string name);
    ObjectMoverSplit(std::vector<std::shared_ptr<ObjectMover>> movers, std::string name);
    ObjectMoverSplit();
    Eigen::Vector3f getPosition(float time);
    float getStartTime();
    float getEndTime();
    MoverType getTag();
    void shift(Eigen::Vector3f shiftValue);
    void addMover(std::shared_ptr<ObjectMover> mover);
    std::vector<std::shared_ptr<ObjectMover>> movers;
    std::shared_ptr<ObjectMover> clone();
    std::shared_ptr<ObjectMoverSplit> cloneAndCast();
    std::string name;
};

class ObjectMoverConstant: public ObjectMover {
public:
    ObjectMoverConstant(Eigen::Vector3f pos, float startTime, float duration);
    Eigen::Vector3f getPosition(float time);
    float getStartTime();
    float getEndTime();
    MoverType getTag();
    void shift(Eigen::Vector3f shiftValue);
    float startTime;
    float endTime;
    std::shared_ptr<ObjectMover> clone();
    Eigen::Vector3f position;
};

class ObjectMoverBezier: public ObjectMover {
public:
    ObjectMoverBezier(const std::vector<Eigen::Vector3f>& points, float startTime, float duration);
    Eigen::Vector3f getPosition(float time);
    float getStartTime();
    float getEndTime();
    MoverType getTag();
    void shift(Eigen::Vector3f shiftValue);
    float startTime;
    float endTime;
    std::vector<Eigen::Vector3f> points;
    std::shared_ptr<ObjectMover> clone();
};


#endif //ASSIGNMENT1_CPP_OBJECTMOVER_H
