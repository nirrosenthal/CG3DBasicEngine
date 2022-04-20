#pragma once
#include "igl/opengl/glfw/Viewer.h"

enum Coefficient {A=0, B=1, C=2, D=3};

class Assignment1 : public igl::opengl::glfw::Viewer
{
public:
	Assignment1(int width, int height);
	void Init();
	void Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model,
                unsigned int  shaderIndx, unsigned int shapeIndx);
	void WhenRotate();
	void WhenTranslate();
	void Animate() override;
	void ScaleAllShapes(float amt, int viewportIndx);
    void increment_IterationNum();
    void decrement_IterationNum();
    void choose_coefficient(Coefficient val);
    void increase_chosen_coefficient();
    void decrease_chosen_coefficient();
    void print_pixel_width();
    void zoom_in();
    void zoom_out();
    void move_vertically(float distance);
    void move_horizontally(float distance);
	~Assignment1();


private:
    int width;
    int height;
    int iterationNum;
    Coefficient chosen_coefficient;
    float a;
    float b;
    float c;
    float d;
    float zoom_ratio;
    float x_offset;
    float y_offset;
};


