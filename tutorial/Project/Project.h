#pragma once
#include "igl/opengl/glfw/Viewer.h"
#include "SceneShape.h"
#include "SceneShader.h"
#include "SceneCamera.h"
#include "igl/opengl/glfw/renderer.h"
#include "ProjectViewerData.h"
#include "BoundingRectangle.h"
#include <stack>

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
class GuiState;

enum AnimationStatus {PLAYING, STOPPED, PAUSED};
enum MouseStatus {NOT_PRESSED, LEFT_CLICK, RIGHT_CLICK};
struct WindowLocation {
    ImVec2 topLeft;
    ImVec2 bottomRight;
};

enum SplitCameraOption {UNSPLIT,SPLITX,SPLITY};
enum ControlledCamera {MAIN, TOP, BOTTOM, LEFT, RIGHT};


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
    SplitCameraOption GetPrevSplitCameraOption();
    void SetPrevSplitCameraOption(SplitCameraOption prevCameraOption);
    std::vector<int> pickedShapes;
    void SetViewportWidth(int w);
    void SetViewportHeight(int w);
    void DeleteShape(std::shared_ptr<SceneShape> shape);
    int VP_Width;
    int VP_Height;
    void ChangeControlledCamera();
    int GetConrolledCameraId();
    std::stack<std::shared_ptr<GuiState>> guiStates;
    void RightClick(float x, float y);
    void LeftClick(float x, float y);
    void UnpressMouse(float x, float y);
    bool IsMousePressed();
    std::shared_ptr<SceneCamera> GetCamera(const std::string& cameraName);
    std::shared_ptr<SceneCamera> GetCamera(const int cameraId);
    int GetCameraId(std::string cameraName);
    std::string GetCameraName(int cameraId);
    std::vector<std::string> GetAllCameras();
    std::shared_ptr<SceneCamera> AddGlobalCamera(std::string _name, float _angle, float _relationWH, float _near, float _far,
                                                 std::shared_ptr<ObjectMoverSplit> _mover);
    std::string GetCameraScreen1();
    std::string GetCameraScreen2();
    std::string GetCameraScreenAnimation();
    void SetCameraScreen1(std::string cameraName);
    void SetCameraScreen2(std::string cameraName);
    void SetCameraScreenAnimation(std::string cameraName);
    bool IsGuiInitialized() const;
    void OpenNewWindow(std::shared_ptr<GuiState> state);
    std::shared_ptr<GuiState> GetCurrentWindow();
    void CloseCurrentWindow();
    std::vector<std::shared_ptr<SceneShape>> GetPickedShapes();
    std::vector<std::shared_ptr<SceneShape>> multiPickedShapes;


private:
    const float FAR = 20000.0f;
    SplitCameraOption previousState;
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
    std::vector<std::string> allCameras;
    std::map<int, std::shared_ptr<SceneCamera>> createdCamerasById;
    std::map<std::string, std::shared_ptr<SceneCamera>> createdCamerasByName;
    void RefreshShadersList();
    void RefreshShapeFilesList();
    long lastFileSystemRefreshingTimeSeconds;
    int backgroundShape[4];
    std::shared_ptr<SceneShader> backgroundShader;
    std::map<std::string, std::shared_ptr<ObjectMoverSplit>> movementCurves;
    Display *display;
    igl::opengl::glfw::imgui::ImGuiMenu* menu;
    SplitCameraOption splitCameraOption;
    SplitCameraOption prevSplitCameraOption;
    void SetSplitCameraOption(SplitCameraOption cameraOption);
    void CalculateShapePosition(int shapeIndex);
    void CalculateShapeSize(int shapeIndex);
    std::unordered_map<int, bool> deletedShapes;
    bool isDeleted(int id);
    ControlledCamera controlledCamera;
    MouseStatus mouseStatus;
    Eigen::Vector2f pressStartPosition;
    void HandleLeftClickEnd(float x, float y);
    void HandleRightClickEnd(float x, float y);
    std::string cameraScreen1;
    std::string cameraScreen2;
    std::string cameraScreenAnimation;
    void UpdatePickingRectangle(BoundingRectangle rec);
    bool shouldDrawPickingRectangle;
    BoundingRectangle pickingRectangle;

};


