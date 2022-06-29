#include "Project.h"
#include <iostream>
#include <chrono>


static void printMat(const Eigen::Matrix4d& mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat(j,i)<<" ";
		std::cout<<std::endl;
	}
}

long getCurrentUnixTime() {
    const auto p1 = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count();
}

IGL_INLINE ProjectViewerData* Project::data(int mesh_id /*= -1*/)
{
    assert(!data_list.empty() && "data_list should never be empty");
    int index;
    if (mesh_id == -1)
        index = selected_data_index;
    else
        index = mesh_index(mesh_id);

    assert((index >= 0 && index < data_list.size()) &&
           "selected_data_index or mesh_id should be in bounds");
    return (ProjectViewerData*)data_list[index];
}

IGL_INLINE const ProjectViewerData* Project::data(int mesh_id /*= -1*/) const
{
    assert(!data_list.empty() && "data_list should never be empty");
    int index;
    if (mesh_id == -1)
        index = selected_data_index;
    else
        index = mesh_index(mesh_id);

    assert((index >= 0 && index < data_list.size()) &&
           "selected_data_index or mesh_id should be in bounds");
    return (ProjectViewerData*)data_list[index];
}

IGL_INLINE int Project::append_mesh(bool visible) {
    assert(data_list.size() >= 1);
    data_list.emplace_back(new ProjectViewerData());
    selected_data_index = data_list.size()-1;
    data_list.back()->id = next_data_id++;
    return data_list.back()->id;
}
Project::Project(): igl::opengl::glfw::Viewer() {
    delete data_list.front();
    data_list.front() = new ProjectViewerData();
}

//Project::Project(float angle ,float relationWH, float near, float far) : Scene(angle,relationWH,near,far)
//{ 	
//}

SceneShape Project::AddGlobalShape(std::string name, igl::opengl::glfw::Viewer::shapes shapeType,
                             std::shared_ptr<ObjectMover> mover, std::shared_ptr<Layer3> layer) {

    int index = AddShape(shapeType, -1, TRIANGLES);
    SceneShape scnShape(name, shapeType, mover, layer,index);
    scnShape.setlastDrawnPosition(Eigen::Vector3f(0,0,0));
    shapesGlobal.push_back(scnShape);
    return scnShape;

}
void Project::Init()
{
    globalTime = -1;
	unsigned int texIDs[3] = { 0 , 1, 2};
	unsigned int slots[3] = { 0 , 1, 2 };
    AddShader("shaders/pickingShader");
    AddShader("shaders/cubemapShader");
    AddShader("shaders/basicShader");
    //AddShader("shaders/pickingShader");
    AddShader("shaders/basicShader2");

    AddTexture("textures/grass.bmp", 2);
    AddTexture("textures/cubemaps/Daylight Box_", 3);
    AddTexture("textures/box0.bmp", 2);
    //AddTexture("../res/textures/Cat_bump.jpg", 2);

    AddMaterial(texIDs,slots, 1);
    AddMaterial(texIDs+1, slots+1, 1);
    AddMaterial(texIDs + 2, slots + 2, 1);
    AddMaterial(texIDs + 3, slots + 3, 1);

    std::vector<Eigen::Vector3f> points = {Eigen::Vector3f(0,0,0),
                                           Eigen::Vector3f(0,20,0),
                                           Eigen::Vector3f(-10,-10,-100),
                                           Eigen::Vector3f(0,0,0)};

    std::vector<Eigen::Vector3f> pointsRev = {Eigen::Vector3f(0,0,0),
                                              Eigen::Vector3f(-10,-10,-100),
                                              Eigen::Vector3f(0,20,0),
                                              Eigen::Vector3f(0,0,0)};

    std::shared_ptr<ObjectMoverBezier> bez = std::make_shared<ObjectMoverBezier>(points, 0, 500);
    SceneShape shp = AddGlobalShape("test", Cube, bez, nullptr);
    shapesGlobal[shp.getIndex()].addMover(std::make_shared<ObjectMoverConstant>(Eigen::Vector3f(0,0,0),
                                                                                500, 50));
    shapesGlobal[shp.getIndex()].addMover(std::make_shared<ObjectMoverBezier>(pointsRev, 550, 500));
//    shp.addMover( std::make_shared<ObjectMoverConstant>(Eigen::Vector3f(0,0,0), 1000, 100));
//    shp.addMover( std::make_shared<ObjectMoverBezier>(points, 2100, 500));
    SetShapeShader(shp.getIndex(), 2);
    SetShapeMaterial(shp.getIndex(), 2);


    animating = false;
    //SetShapeViewport(6, 1);
//	ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Project::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx)
{
    ++globalTime;
    if(globalTime == 100) {
//        std::list<int> x, y;
//        const int DISPLAY_WIDTH = 1200;
//        const int DISPLAY_HEIGHT = 800;
//        x.push_back(DISPLAY_WIDTH);
//        y.push_back(DISPLAY_HEIGHT);
//        igl::opengl::glfw::imgui::ImGuiMenu* menu = new igl::opengl::glfw::imgui::ImGuiMenu();
//        renderer->Init(this,x,y,1,menu);
    }
	Shader *s = shaders[shaderIndx];
    long time;
    if(animating)
        time = globalTime;
    else
        time = 0;
	int r = ((shapeIndx+1) & 0x000000FF) >>  0;
	int g = ((shapeIndx+1) & 0x0000FF00) >>  8;
	int b = ((shapeIndx+1) & 0x00FF0000) >> 16;

    SceneShape scnShape = shapesGlobal[shapeIndx];
    Eigen::Vector3f pos = scnShape.getlastDrawnPosition();
    std::cout << "(" << pos[0] << "," << pos[1] << "," << pos[2] << ")" << std::endl;

    Eigen::Vector3f newPos = scnShape.getPosition((float)time);
    Eigen::Vector3f delta = newPos - pos;

    pickedShapes = {scnShape.getIndex()};
    ShapeTransformation(xTranslate, delta[0],0);
    ShapeTransformation(yTranslate, delta[1],0);
    ShapeTransformation(zTranslate, delta[2],0);
    shapesGlobal[shapeIndx].setlastDrawnPosition(newPos);
    pickedShapes.clear();



		s->Bind();
	s->SetUniformMat4f("Proj", Proj);
	s->SetUniformMat4f("View", View);
	s->SetUniformMat4f("Model", Model);
	if (data_list[shapeIndx]->GetMaterial() >= 0 && !materials.empty())
	{
//		materials[shapes[pickedShape]->GetMaterial()]->Bind(textures);
		BindMaterial(s, data_list[shapeIndx]->GetMaterial());
	}
	if (shaderIndx == 0)
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
	else
		s->SetUniform4f("lightColor", 4/100.0f, 60 / 100.0f, 99 / 100.0f, 0.5f);
	//textures[0]->Bind(0);

	
	

	//s->SetUniform1i("sampler2", materials[shapes[pickedShape]->GetMaterial()]->GetSlot(1));
	//s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
//	if(shaderIndx == 0)
//		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
//	else 
//		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	s->Unbind();
}


void Project::WhenRotate()
{
}

void Project::WhenTranslate()
{
}

void Project::Animate() {

    if(isActive)
	{
		if(selected_data_index > 0 )
			data()->MyRotate(Eigen::Vector3d(0, 1, 0), 0.01);
	}
}

void Project::ScaleAllShapes(float amt,int viewportIndx)
{
	for (int i = 1; i < data_list.size(); i++)
	{
		if (data_list[i]->Is2Render(viewportIndx))
		{
            data_list[i]->MyScale(Eigen::Vector3d(amt, amt, amt));
		}
	}
}

Project::~Project(void)
{
}

void Project::SetRenderer(Renderer *renderer) {
    if(this->renderer != nullptr)
        throw std::invalid_argument("renderer cannot be set twice");
    this->renderer = renderer;

}

void Project::SetParent(int shape, int newParent) {
    shapesGlobal[shapesGlobal[shape].getParent()].removeChild(shape);
    shapesGlobal[shape].setParent(newParent);
    shapesGlobal[newParent].addChild(shape);
}

int Project::GetParent(int shape) {
    return shapesGlobal[shape].getParent();
}

std::vector<int> Project::GetChildren(int shape) {
    return shapesGlobal[shape].getChildren();
}

void Project::Play() {
    animating = !animating;
    globalTime = -1;
}




