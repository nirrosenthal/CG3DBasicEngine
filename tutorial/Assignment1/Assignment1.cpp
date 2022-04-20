#include "Assignment1.h"
#include <iostream>

std::complex<float> NewtonCubicRoot(std::complex<float> num)
{
    std::complex<float> root = num;
    const int iter = 7;
    for (size_t k = 0; k < iter && root != 0.0f; k++)
    {
        root = (2.0f*root*root*root + num)/root/root/3.0f;
    }
    return root;
}

Eigen::Vector3cf FindRootsOfReduceEquation(Eigen::Vector2cf reduceCoeffs)
{
    Eigen::Vector3cf roots = Eigen::Vector3cf::Zero();
    std::complex<float> sqroot = std::sqrt(reduceCoeffs[0]*reduceCoeffs[0]*reduceCoeffs[0]/27.0f + reduceCoeffs[1]*reduceCoeffs[1]/4.0f);
    std::complex<float> p = NewtonCubicRoot(reduceCoeffs[1]/2.0f + sqroot);
    std::complex<float> n = NewtonCubicRoot(reduceCoeffs[1]/2.0f - sqroot);
    roots[0] = p + n;
    roots[1] = p *std::complex<float>(cosf(2.0f*3.14159f/3.0f),sinf(2*3.14159f/3.0f)) - n * std::complex<float>(cosf(1.0f*3.14159f/3.0f),sinf(1*3.14159f/3.0f));
    roots[2] = -p * std::complex<float>(cosf(1.0f*3.14159f/3.0f),sinf(1*3.14159f/3.0f)) + n * std::complex<float>(cosf(2.0f*3.14159f/3.0f),sinf(2*3.14159f/3.0f));
    return roots;
}

Eigen::Vector3cf FindCubicRoots(std::complex<float> a, std::complex<float> b, std::complex<float> c, std::complex<float> d)
{
    Eigen::Vector2cf reduceCoeffs = Eigen::Vector2cf::Zero();
    Eigen::Vector3cf roots;
    std::complex<float> bOver3a = (b/a)/3.0f;
    reduceCoeffs[0] = c/a - 3.0f*bOver3a*bOver3a;
    reduceCoeffs[1] = c/a*bOver3a - d/a - 2.0f*bOver3a*bOver3a*bOver3a;
    if(reduceCoeffs.norm()>0.000001)
    {
        roots =	FindRootsOfReduceEquation(reduceCoeffs);
        roots[0] -= bOver3a;
        roots[1] -= bOver3a;
        roots[2] -= bOver3a;
    }
    else
    {
        roots[0] = -1.0f*bOver3a;
        roots[1] = std::complex<float>(cosf(3.14159f/3.0f),sinf(3.14159f/3.0f))*bOver3a;
        roots[2] = std::complex<float>(cosf(2.0f*3.14159f/3.0f),sinf(2*3.14159f/3.0f))*bOver3a;
    }

    return roots;
}


Assignment1::Assignment1(int width, int height): width(width), height(height), iterationNum(1), a(1), b(0), c(0), d(-1),
    chosen_coefficient(Coefficient::A), zoom_ratio(1.0f), x_offset(0.0f), y_offset(0.0f), mouse_x(0.0f), mouse_y(0.0f) {}

void Assignment1::Init()
{

	unsigned int texIDs[3] = { 0 , 1, 2};
	unsigned int slots[3] = { 0 , 1, 2 };

    int shader = AddShader("shaders/newton");


    int shape = AddShape(Plane, -1, TRIANGLES);
    int met = AddMaterial(texIDs,slots, 1);
    SetShapeShader(shape, shader);
    SetShapeMaterial(shape, met);
    shaders[shader]->Bind();
    Eigen::Vector3cf roots = FindCubicRoots(std::complex<float>(a), std::complex<float>(b),
            std::complex<float>(c), std::complex<float>(d));

    shaders[shader]->SetUniform4f("roots_real", roots[0].real(), roots[1].real(),
                    roots[2].real(), 0.0f);
    shaders[shader]->SetUniform4f("roots_img", roots[0].imag(), roots[1].imag(),
                    roots[2].imag(), 0.0f);
    shaders[shader]->SetUniform1f("zoom_ratio", zoom_ratio);
    shaders[shader]->SetUniform1f("x_offset", x_offset);
    shaders[shader]->SetUniform1f("y_offset", y_offset);
    shaders[shader]->SetUniform4f("coefficients", a, b, c, d);

    shaders[shader]->SetUniform1i("num_of_iterations", iterationNum);
    shaders[shader]->Unbind();
    SetShapeStatic(shape);
}

void Assignment1::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model,
                         unsigned int  shaderIndx, unsigned int shapeIndx)
{
	Shader *s = shaders[shaderIndx];

	s->Bind();
	s->SetUniformMat4f("Proj", Proj);
	s->SetUniformMat4f("View", View);
	s->SetUniformMat4f("Model", Model);
    Eigen::Vector3cf roots = FindCubicRoots(a, b, c, d);
    s->SetUniform4f("roots_real", roots[0].real(), roots[1].real(),
                                  roots[2].real(), 0.0f);
    s->SetUniform4f("roots_img", roots[0].imag(), roots[1].imag(),
                                  roots[2].imag(), 0.0f);
    s->SetUniform4f("coefficients", a, b, c, d);
    s->SetUniform1i("num_of_iterations", iterationNum);
    s->SetUniform1f("zoom_ratio", zoom_ratio);
    s->SetUniform1f("x_offset", x_offset);
    s->SetUniform1f("y_offset", y_offset);

	s->Unbind();
}


void Assignment1::WhenRotate()
{
}

void Assignment1::WhenTranslate()
{
}

void Assignment1::Animate() {
    if(isActive)
	{
		
	}
}

void Assignment1::ScaleAllShapes(float amt,int viewportIndx)
{
	for (int i = 1; i < data_list.size(); i++)
	{
		if (data_list[i]->Is2Render(viewportIndx))
		{
            data_list[i]->MyScale(Eigen::Vector3d(amt, amt, amt));
		}
	}
}

Assignment1::~Assignment1(void)
{
}

void Assignment1::choose_coefficient(Coefficient val) { chosen_coefficient = val; }

void Assignment1::increment_IterationNum() { iterationNum++; }

void Assignment1::decrement_IterationNum() { if(iterationNum > 1) iterationNum--; }

void Assignment1::increase_chosen_coefficient() {
    switch(chosen_coefficient) {
        case Coefficient::A:
            a+=COEFFICIENT_CHANGE_VALUE;
            break;
        case Coefficient::B:
            b+=COEFFICIENT_CHANGE_VALUE;
            break;
        case Coefficient::C:
            c+=COEFFICIENT_CHANGE_VALUE;
            break;
        case Coefficient::D:
            d+=COEFFICIENT_CHANGE_VALUE;
            break;
    }
}

void Assignment1::decrease_chosen_coefficient() {
    switch(chosen_coefficient) {
        case Coefficient::A:
            a-=COEFFICIENT_CHANGE_VALUE;
            break;
        case Coefficient::B:
            b-=COEFFICIENT_CHANGE_VALUE;
            break;
        case Coefficient::C:
            c-=COEFFICIENT_CHANGE_VALUE;
            break;
        case Coefficient::D:
            d-=COEFFICIENT_CHANGE_VALUE;
            break;
    }
}
void Assignment1::print_pixel_width()
{
    std::cout << "each pixel is " << zoom_ratio << " points wide." << std::endl;
}

void Assignment1::zoom_in()
{
    zoom_ratio*=ZOOM_EXP_FACTOR;
    print_pixel_width();
}

void Assignment1::zoom_out()
{
    zoom_ratio/=ZOOM_EXP_FACTOR;
    print_pixel_width();
}

void Assignment1::move_vertically(float offset) {y_offset+=offset*zoom_ratio;}

void Assignment1::move_horizontally(float offset) {x_offset+=offset*zoom_ratio;}

void Assignment1::update_mouse_on_click(float x, float y)
{
    move_horizontally((mouse_x-x)/(float)width);
    move_vertically((y-mouse_y)/(float)height);
    update_mouse_coordinates(x, y);
}

void Assignment1::update_mouse_with_no_click(float x, float y) {update_mouse_coordinates(x, y);}

void Assignment1::update_mouse_coordinates(float x, float y)
{
    mouse_x = x;
    mouse_y = y;
}


void Assignment1::resize(int width, int height)
{
    this->width = width;
    this->height = height;
}



