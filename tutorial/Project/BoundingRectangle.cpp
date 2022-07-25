//
// Created by nadavhai@wincs.cs.bgu.ac.il on 7/25/22.
//

#include "BoundingRectangle.h"

#include <utility>

BoundingRectangle::BoundingRectangle(Eigen::Vector2f point1, Eigen::Vector2f point2) {
    if(point1.x() == point2.x())
        throw std::runtime_error("Cannot construct a bounding rectangle for two points with the same x value!");
    if(point1.y() == point2.y())
        throw std::runtime_error("Cannot construct a bounding rectangle for two points with the same y value!");

    if(point1.x() < point2.x() && point1.y() < point2.y()) {
        bottomLeft = point1;
        topRight = point2;
    } else if(point1.x() < point2.x() && point1.y() > point2.y()) {
        bottomLeft = Eigen::Vector2f(point1.x(), point2.y());
        topRight = Eigen::Vector2f(point2.x(), point1.y());
    } else if(point1.x() > point2.x() && point1.y() < point2.y()) {
        bottomLeft = Eigen::Vector2f(point2.x(), point1.y());
        topRight = Eigen::Vector2f(point1.x(), point2.y());
    } else { // point1.x() > point2.x() && point1.y() > point2.y()
        bottomLeft = point2;
        topRight = point1;
    }
}
BoundingRectangle::BoundingRectangle() = default;

bool BoundingRectangle::Contains(Eigen::Vector2f point) {
    return bottomLeft.x() <= point.x() && point.x() <= topRight.x() &&
        bottomLeft.y() <= point.y() && point.y() <= topRight.y();
}

Eigen::Vector2f BoundingRectangle::BottomLeft() const {
    return bottomLeft;
}

Eigen::Vector2f BoundingRectangle::BottomRight() const {
    return Eigen::Vector2f(topRight.x(), bottomLeft.y());
}

Eigen::Vector2f BoundingRectangle::TopLeft() const {
    return Eigen::Vector2f(bottomLeft.x(), topRight.y());
}

Eigen::Vector2f BoundingRectangle::TopRight() const {
    return topRight;
}

