#include "Assignment1.h"
#include <iostream>


Assignment1::Assignment1(int width, int height): width(width), height(height), iterationNum(1000),
    a(1), b(0), c(0), d(-1), chosen_coefficient(Coefficient::A)
{}

void Assignment1::Init()
{		
	unsigned int texIDs[3] = { 0 , 1, 2};
	unsigned int slots[3] = { 0 , 1, 2 };

//    Eigen::Vector4f *colorMat = colorMatrix(width, height, a, b, c, d);

    int shader = AddShader("shaders/newton");


    int shape = AddShape(Cube, -1, TRIANGLES);
    int met = AddMaterial(texIDs,slots, 1);
    int snake = AddTexture("textures/snake.jpg", 0);
    SetShapeShader(shape, shader);
    SetShapeMaterial(shape, met);

    shaders[shader]->Bind();
    shaders[shader]->SetUniform4f("resolution", (float)width, (float)height, 0.0f, 0.0f);
    shaders[shader]->SetUniform1f("a", a);
    shaders[shader]->SetUniform1f("b", b);
    shaders[shader]->SetUniform1f("c", c);
    shaders[shader]->SetUniform1f("d", d);
    shaders[shader]->SetUniform1i("num_of_iterations", iterationNum);
    shaders[shader]->Unbind();
    SetShapeStatic(shape);
//	ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Assignment1::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model,
                         unsigned int  shaderIndx, unsigned int shapeIndx)
{
	Shader *s = shaders[shaderIndx];

	s->Bind();
	s->SetUniformMat4f("Proj", Proj);
	s->SetUniformMat4f("View", View);
	s->SetUniformMat4f("Model", Model);
    s->SetUniform1f("a", a);
    s->SetUniform1f("b", b);
    s->SetUniform1f("c", c);
    s->SetUniform1f("d", d);
    s->SetUniform1i("num_of_iterations", iterationNum);
	if (data_list[shapeIndx]->GetMaterial() >= 0 && !materials.empty())
	{
//		materials[shapes[pickedShape]->GetMaterial()]->Bind(textures);
		BindMaterial(s, data_list[shapeIndx]->GetMaterial());
	}

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
            a+=0.1;
            break;
        case Coefficient::B:
            b+=0.1;
            break;
        case Coefficient::C:
            c+=0.1;
            break;
        case Coefficient::D:
            d+=0.1;
            break;
    }
}

void Assignment1::decrease_chosen_coefficient() {
    switch(chosen_coefficient) {
        case Coefficient::A:
            a-=0.1;
            break;
        case Coefficient::B:
            b-=0.1;
            break;
        case Coefficient::C:
            c-=0.1;
            break;
        case Coefficient::D:
            d-=0.1;
            break;
    }
}


