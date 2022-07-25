//
// Created by nadavhai@wincs.cs.bgu.ac.il on 7/25/22.
//

#ifndef ENIGEREWORK_BOUNDINGRECTANGLE_H
#define ENIGEREWORK_BOUNDINGRECTANGLE_H


#include <Eigen/Core>

class BoundingRectangle {
public:
    BoundingRectangle(Eigen::Vector2f point1, Eigen::Vector2f point2);
    BoundingRectangle();
    bool Contains(Eigen::Vector2f point);
    Eigen::Vector2f BottomLeft() const;
    Eigen::Vector2f BottomRight() const;
    Eigen::Vector2f TopLeft() const;
    Eigen::Vector2f TopRight() const;

private:
    Eigen::Vector2f bottomLeft;
    Eigen::Vector2f topRight;
};


#endif //ENIGEREWORK_BOUNDINGRECTANGLE_H
