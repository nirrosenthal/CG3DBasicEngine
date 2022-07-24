#pragma once
#include "igl/opengl/glfw/Viewer.h"
#include "SceneShape.h"
#include "SceneShader.h"
#include "igl/opengl/glfw/renderer.h"
#include "ProjectViewerData.h"

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

class Gui;

enum AnimationStatus {PLAYING, STOPPED, PAUSED};
struct WindowLocation {
    ImVec2 topLeft;
    ImVec2 bottomRight;
};

enum SplitCameraOption {UNSPLIT,SPLITX,SPLITY};


const std::string SHADERS_FOLDER = "shaders/";
const std::string SHAPES_FOLDER = "data/";
const std::string TEXTURES_FOLDER = "textures/";
class Project : public igl::opengl::glfw::Viewer
{
	
public:

    IGL_INLINE ProjectViewerData* data(int mesh_id = -1);
    IGL_INLINE const ProjectViewerData* data(int mesh_id = -1) const;
    IGL_INLINE int append_mesh(bool visible = true);
    IGL_INLINE void Draw(int shaderIndx, const Eigen::Matrix4f &Proj, const Eigen::Matrix4f &View,
                         int viewportIndx, unsigned int flgs,unsigned int property_id) override;

	Project(igl::opengl::glfw::imgui::ImGuiMenu* menu);
//	Project(float angle,float relationWH,float near, float far);
	void Init(float width, float height);
	void Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx);
	void WhenRotate();
	void WhenTranslate();
	void Animate() override;
	void ScaleAllShapes(float amt, int viewportIndx);
    void SetRenderer(Renderer *renderer);
    Renderer *GetRenderer() {return renderer;};
    void SetDisplay(Display *display);
    void SetParent(int shape, int newParent);
    int GetParent(int shape);
    std::vector<int> GetChildren(int shape);
    void Play();
    void Pause();
    void Stop();
    void Replay();
    float maxTime();
    AnimationStatus getAnimationStatus();
	~Project(void);
    long GetGlobalTime();
    void SetGlobalTime(long time);
    void UpdateWindowLocation(ImVec2 topLeft, ImVec2 bottomRight);
    LayerManager layerManager;
    WindowLocation GetWindowLocation() {return windowLocation;};
    std::vector<std::string> GetAllShaders();
    std::string GetShaderName(int shaderId);
    int GetShaderId(std::string shaderName);
    std::vector<std::shared_ptr<SceneShape>> getAllShapes();
    std::shared_ptr<SceneShape> AddGlobalShape(std::string name, shapes shapeType, std::shared_ptr<ObjectMoverSplit> mover,
                                               std::shared_ptr<Layer> layer, std::string shader);
    std::shared_ptr<SceneShape> AddGlobalShape(std::string name, std::string file, std::shared_ptr<ObjectMoverSplit> mover,
                                               std::shared_ptr<Layer> layer, std::string shader);
    std::shared_ptr<SceneShape> GetGlobalShape(const std::string& name);
    bool AddGlobalShader(std::shared_ptr<SceneShader> shader);
    std::shared_ptr<SceneShader> GetShader(const std::string& shaderName);
    std::shared_ptr<SceneShader> GetShader(const int shaderId);
    void UpdateResolution(float width, float height);
    void UpdateMouse(float x, float y);
    Eigen::Vector2f GetMouse();
    std::string GetBackgroundShader();
    void SetBackgroundShader(std::string shaderName);
    std::vector<std::string> GetAllMovementCurves();
    bool AddMovementCurve(std::shared_ptr<ObjectMoverSplit> curve);
    void SetNewMoversForCurve(std::string name, std::vector<std::shared_ptr<ObjectMover>> newMovers);
    std::shared_ptr<ObjectMoverSplit> GetCurve(std::string name);
    void SetShapeCurve(int shapeId, std::string curveName);
    void SplitX();
    void SplitY();
    void Unsplit();
    std::vector<std::string> GetAllShapeFiles();
    SplitCameraOption GetSplitCameraOption();
    std::vector<int> pickedShapes;
    void SetViewportWidth(int w);
    void SetViewportHeight(int w);
    void DeleteShape(std::shared_ptr<SceneShape> shape);
    int VP_Width;
    int VP_Height;


private:
    Renderer *renderer = nullptr;
    long globalTime;
    std::map<int, std::shared_ptr<SceneShape>> shapesGlobal;
    WindowLocation windowLocation;
    Eigen::Vector2f resolution;
    Eigen::Vector2f mousePos;
    AnimationStatus animationStatus;
    std::map<std::string, std::shared_ptr<SceneShader>> createdShadersByName;
    std::map<int, std::shared_ptr<SceneShader>> createdShadersById;
    std::vector<std::string> allShaders;
    std::vector<std::string> allShapeFiles;
    void RefreshShadersList();
    void RefreshShapeFilesList();
    long lastFileSystemRefreshingTimeSeconds;
    int backgroundShape[4];
    std::shared_ptr<SceneShader> backgroundShader;
    std::map<std::string, std::shared_ptr<ObjectMoverSplit>> movementCurves;
    Display *display;
    igl::opengl::glfw::imgui::ImGuiMenu* menu;
    SplitCameraOption splitCameraOption;
    void SetSplitCameraOption(SplitCameraOption camera_option);
    void CalculateShapePosition(int shapeIndex);
    void CalculateShapeSize(int shapeIndex);
    std::unordered_map<int, bool> deletedShapes;
    bool isDeleted(int id);

};


