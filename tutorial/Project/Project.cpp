#include "Project.h"
#include <iostream>
#include <chrono>

#  if __has_include(<filesystem>)
#    include <filesystem>
#    define USING_BOOST false
     namespace filesystem = std::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
#    include <boost/filesystem.hpp>
#    define USING_BOOST true
namespace filesystem = boost::filesystem;
#  elif __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
#    define USING_BOOST false
namespace filesystem = std::experimental::filesystem;
#  endif



bool endsWith (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}
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
    return std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
}

IGL_INLINE void Project::Draw(int shaderIndx, const Eigen::Matrix4f &Proj, const Eigen::Matrix4f &View, int viewportIndx, unsigned int flgs,unsigned int property_id)
{
    if(animationStatus == PLAYING && globalTime < maxTime())
        ++globalTime;

    Eigen::Matrix4f Normal;

    if (!(staticScene & (1<<viewportIndx)))
        Normal = MakeTransScale();
    else
        Normal = Eigen::Matrix4f::Identity();

    for (int i = 0; i < data_list.size(); i++)
    {
        if(i != backgroundShape && !shapesGlobal[i]->isDrawn(globalTime))
            continue;
        auto shape = data_list[i];
        if (shape->Is2Render(viewportIndx))
        {

            Eigen::Matrix4f Model = shape->MakeTransScale();

            if (!shape->IsStatic())
            {

                Model = Normal * GetPriviousTrans(View.cast<double>(),i).cast<float>() * Model;
            }
            else if (parents[i] == -2) {
                Model = View.inverse() * Model;
            }
            if (!(flgs & 65536))
            {
                Update(Proj, View, Model, shape->GetShader(),i);
                // Draw fill
                if (shape->show_faces & property_id)
                    shape->Draw(shaders[shape->GetShader()], true);
                if (shape->show_lines & property_id) {
                    glLineWidth(shape->line_width);
                    shape->Draw(shaders[shape->GetShader()],false);
                }
                // overlay draws
                if(shape->show_overlay & property_id){
                    if (shape->show_overlay_depth & property_id)
                        glEnable(GL_DEPTH_TEST);
                    else
                        glDisable(GL_DEPTH_TEST);
                    if (shape->lines.rows() > 0)
                    {
                        Update_overlay(Proj, View, Model,i,false);
                        glEnable(GL_LINE_SMOOTH);
                        shape->Draw_overlay(overlay_shader,false);
                    }
                    if (shape->points.rows() > 0)
                    {
                        Update_overlay(Proj, View, Model,i,true);
                        shape->Draw_overlay_pints(overlay_point_shader,false);
                    }
                    glEnable(GL_DEPTH_TEST);
                }
            }
            else
            { //picking
                if (flgs & 16384)
                {   //stencil
                    Eigen::Affine3f scale_mat = Eigen::Affine3f::Identity();
                    scale_mat.scale(Eigen::Vector3f(1.1f, 1.1f, 1.1f));
                    Update(Proj, View , Model * scale_mat.matrix(), 0,i);
                }
                else
                {
                    Update(Proj, View ,  Model, 0,i);
                }
                shape->Draw(shaders[0], true);
            }
        }
    }
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

std::shared_ptr<SceneShape> Project::AddGlobalShape(std::string name, igl::opengl::glfw::Viewer::shapes shapeType,
                             std::shared_ptr<ObjectMoverSplit> mover, std::shared_ptr<Layer> layer, std::string shader) {

    int index = AddShape(shapeType, -1, TRIANGLES);
    std::shared_ptr<SceneShape> scnShape = std::make_shared<SceneShape>(name, shapeType, mover, layer,index);
    scnShape->setlastDrawnPosition(Eigen::Vector3f(0,0,0));
    layer->addShape(scnShape);
    scnShape->shader = GetShader(shader)->getId();
    scnShape->material = 0;
    shapesGlobal[index] = scnShape;
    return scnShape;

}

void Project::Init(float width, float height)
{
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    lastFileSystemRefreshingTimeSeconds = 0;
    resolution = Eigen::Vector2f(width, height);
    globalTime = 0;
	unsigned int texIDs[3] = { 1 , 2, 3};
	unsigned int slots[3] = { 1 , 2, 3 };


    AddShader("shaders/pickingShader");
    AddShader("shaders/cubemapShader");
    int shader = AddShader("shaders/basicShader");
    //AddShader("shaders/pickingShader");
    AddShader("shaders/basicShader2");

    AddTexture("textures/grass.bmp", 2);
    AddTexture("textures/cubemaps/Daylight Box_", 3);
    int box = AddTexture("textures/box0.bmp", 2);
    //AddTexture("../res/textures/Cat_bump.jpg", 2);

    int mat1 = AddMaterial(texIDs,slots, 1);
    int mat2 = AddMaterial(texIDs+1, slots+1, 1);
    int mat3 = AddMaterial(texIDs + 2, slots + 2, 1);
    int mat4 = AddMaterial(texIDs + 3, slots + 3, 1);

    std::vector<Eigen::Vector3f> points = {Eigen::Vector3f(0,0,0),
                                           Eigen::Vector3f(0,20,0),
                                           Eigen::Vector3f(-10,-10,-100),
                                           Eigen::Vector3f(0,0,0)};

    std::vector<Eigen::Vector3f> pointsRev = {Eigen::Vector3f(0,0,0),
                                              Eigen::Vector3f(-10,-10,-100),
                                              Eigen::Vector3f(0,20,0),
                                              Eigen::Vector3f(0,0,0)};


    std::shared_ptr<ObjectMoverBezier> bez = std::make_shared<ObjectMoverBezier>(points, 0, 500);
    auto constMover = std::make_shared<ObjectMoverConstant>(Eigen::Vector3f(0,0,0), 500, 50);
    std::shared_ptr<ObjectMoverBezier> bezRev = std::make_shared<ObjectMoverBezier>(pointsRev, 550, 500);

    std::vector<std::shared_ptr<ObjectMover>> movers = {bez, constMover, bezRev};
    AddMovementCurve(std::make_shared<ObjectMoverSplit>(movers, "default"));
    std::shared_ptr<ObjectMoverSplit> mover = GetCurve("default");
    auto defaultLayer = layerManager.addLayer("default");
    std::shared_ptr<SceneShape> shp = AddGlobalShape("test", Cube, mover, defaultLayer, "basicShader");

    shp->material = mat1;

    backgroundShape = AddShape(Cube, -1, TRIANGLES);
    selected_data_index = backgroundShape;
    ShapeTransformation(scaleAll, 80, 0);
//    selected_data_index = -1;


    SetShapeStatic(backgroundShape);
    SetBackgroundShader("distorted");



    animationStatus = STOPPED;
    //SetShapeViewport(6, 1);
//	ReadPixel(); //uncomment when you are reading from the z-buffer
}


float Project::maxTime() {
    float maxTime = -1;
    for(std::shared_ptr<SceneShape> s : getAllShapes()) {
        maxTime = std::max(maxTime, s->getEndTime());
    }
    return maxTime;
}


void Project::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx)
{


    long time;
    if(globalTime != -1)
        time = globalTime;
    else
        time = 0;
	int r = ((shapeIndx+1) & 0x000000FF) >>  0;
	int g = ((shapeIndx+1) & 0x0000FF00) >>  8;
	int b = ((shapeIndx+1) & 0x00FF0000) >> 16;
    Shader *s = shaders[shaderIndx];
    if(shapeIndx != backgroundShape) {
        std::shared_ptr<SceneShape> scnShape = shapesGlobal[shapeIndx];
        SetShapeShader(shapeIndx, scnShape->shader);
        SetShapeMaterial(shapeIndx, scnShape->material);
        selected_data_index = scnShape->getIndex();
        Eigen::Vector3f pos = scnShape->getlastDrawnPosition();
        Eigen::Vector3f newPos = scnShape->getPosition((float) time);
        Eigen::Vector3f delta = newPos - pos;
        ShapeTransformation(xTranslate, delta[0], 0);
        ShapeTransformation(yTranslate, delta[1], 0);
        ShapeTransformation(zTranslate, delta[2], 0);
        shapesGlobal[shapeIndx]->setlastDrawnPosition(newPos);
    }

	s->Bind();
	s->SetUniformMat4f("Proj", Proj);
	s->SetUniformMat4f("View", View);
	s->SetUniformMat4f("Model", Model);
    if(createdShadersById.find(shaderIndx) != createdShadersById.end())
        createdShadersById[shaderIndx]->uploadAllUniforms(globalTime, resolution, mousePos);
    else if(backgroundShader != nullptr &&  backgroundShader->getId() == shaderIndx) {
        backgroundShader->uploadAllUniforms(globalTime, resolution, mousePos);
    }
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
    shapesGlobal[shapesGlobal[shape]->getParent()]->removeChild(shape);
    shapesGlobal[shape]->setParent(newParent);
    shapesGlobal[newParent]->addChild(shape);
}

int Project::GetParent(int shape) {
    return shapesGlobal[shape]->getParent();
}
long Project::GetGlobalTime() {
    return globalTime;
}

std::vector<int> Project::GetChildren(int shape) {
    return shapesGlobal[shape]->getChildren();
}

AnimationStatus Project::getAnimationStatus() {return animationStatus;}

void Project::Play() {
    animationStatus = PLAYING;
}

void Project::Pause() {
    animationStatus = PAUSED;
}

void Project::Stop() {
    globalTime = 0;
    animationStatus = STOPPED;
}

void Project::Replay() {
    globalTime = 0;
    animationStatus = PLAYING;
}

void Project::SetGlobalTime(long time) {
    globalTime = time;
}

void Project::UpdateWindowLocation(ImVec2 topLeft, ImVec2 bottomRight)
{
    windowLocation.topLeft = topLeft;
    windowLocation.bottomRight = bottomRight;
}

std::shared_ptr<SceneShader> Project::GetShader(const std::string& shaderName) {
    if(createdShadersByName.find(shaderName) != createdShadersByName.end())
        return createdShadersByName[shaderName];
    std::string shaderPath = SHADERS_FOLDER + shaderName;
    int shaderId = AddShader(shaderPath);
    std::shared_ptr<SceneShader> shader = nullptr;
    if(shaderId != -1) {
        if(AddGlobalShader(std::make_shared<SceneShader>(shaderName, shaderId, shaders[shaderId], SHADERS_FOLDER)))
            shader = createdShadersById[shaderId];
    }
    return shader;
}
std::shared_ptr<SceneShader> Project::GetShader(const int shaderId) {
    if(createdShadersById.find(shaderId) == createdShadersById.end())
        return nullptr;
    return createdShadersById[shaderId];
}


std::vector<std::string> Project::GetAllShaders() {
    long currentTime = getCurrentUnixTime();
    if(currentTime - lastFileSystemRefreshingTimeSeconds > 60) {
        RefreshShadersList();
        lastFileSystemRefreshingTimeSeconds = currentTime;
    }

    return allShaders;
}

void Project::RefreshShadersList() {
    allShaders.clear();
    for(auto const &file : filesystem::directory_iterator(SHADERS_FOLDER)) {
        std::string path;
        # if !USING_BOOST
            path = file.path().u8string();
        # else
            path = file.path().string();
        # endif
        if(endsWith(path, ".glsl")) {
            path = path.substr(0, path.find_last_of('.'));
            path = path.substr(SHADERS_FOLDER.length(), path.length());
            allShaders.push_back(path);
        }
    }
    std::sort(allShaders.begin(), allShaders.end());

}

std::vector<std::shared_ptr<SceneShape>> Project::getAllShapes() {
    std::vector<std::shared_ptr<SceneShape>> allShapes;
    for(auto entry: shapesGlobal)
        allShapes.push_back(entry.second);
    return allShapes;
}

std::shared_ptr<SceneShape> Project::GetGlobalShape(const std::string& name) {
    for(auto &shape : getAllShapes()) {
        if(shape->name == name)
            return shape;
    }
    return nullptr;
}

std::string Project::GetShaderName(int shaderId) {
    if(shaderId == -1)
        return "";
    return createdShadersById[shaderId]->getName();
}
int Project::GetShaderId(std::string shaderName) {
    auto shader = GetShader(shaderName);
    if(shader == nullptr)
        return -1;
    return shader->getId();
}

bool Project::AddGlobalShader(std::shared_ptr<SceneShader> shader) {
    if(createdShadersById.find(shader->getId()) != createdShadersById.end() ||
        createdShadersByName.find(shader->getName()) != createdShadersByName.end())
        return false;

    createdShadersByName[shader->getName()] = shader;
    createdShadersById[shader->getId()] = shader;
    return true;
}

void Project::UpdateResolution(float width, float height) {
    resolution = Eigen::Vector2f(width, height);
}

void Project::UpdateMouse(float x, float y) {
    mousePos = Eigen::Vector2f(x, y);
}

void Project::SetBackgroundShader(std::string shaderName) {
    backgroundShader = GetShader(shaderName);
    SetShapeShader(backgroundShape, backgroundShader->getId());
}

std::string Project::GetBackgroundShader() {
    if(backgroundShader == nullptr)
        return "";
    return backgroundShader->getName();
}

std::vector<std::string> Project::GetAllMovementCurves() {
    std::vector<std::string> curves;
    for(auto &entry : movementCurves)
        curves.push_back(entry.first);
    return curves;
}

bool Project::AddMovementCurve(std::shared_ptr<ObjectMoverSplit> curve) {
    if(movementCurves.find(curve->name) != movementCurves.end())
        return false;
    movementCurves[curve->name] = curve;
    return true;
}

void Project::SetNewMoversForCurve(std::string name, std::vector<std::shared_ptr<ObjectMover>> newMovers) {
    movementCurves[name]->movers = newMovers;
}

std::shared_ptr<ObjectMoverSplit> Project::GetCurve(std::string name) {
    if(movementCurves.find(name) == movementCurves.end())
        return nullptr;
    return movementCurves[name];
}

void Project::SetShapeCurve(int shapeId, std::string curveName) {
    shapesGlobal[shapeId]->mover = movementCurves[curveName];
}

Eigen::Vector2f Project::GetMouse() {
    return mousePos;
}


