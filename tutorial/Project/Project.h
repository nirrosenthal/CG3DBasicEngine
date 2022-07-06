#pragma once
#include "igl/opengl/glfw/Viewer.h"
#include "SceneShape.h"
#include "SceneShader.h"
#include "igl/opengl/glfw/renderer.h"
#include "ProjectViewerData.h"
#include <filesystem>

enum AnimationStatus {PLAYING, STOPPED, PAUSED};
struct WindowLocation {
    ImVec2 topLeft;
    ImVec2 bottomRight;
};

const std::string SHADERS_FOLDER = "shaders/";
const std::string TEXTURES_FOLDER = "textures/";
class Project : public igl::opengl::glfw::Viewer
{
	
public:

    IGL_INLINE ProjectViewerData* data(int mesh_id = -1);
    IGL_INLINE const ProjectViewerData* data(int mesh_id = -1) const;
    IGL_INLINE int append_mesh(bool visible = true);
    IGL_INLINE void Draw(int shaderIndx, const Eigen::Matrix4f &Proj, const Eigen::Matrix4f &View,
                         int viewportIndx, unsigned int flgs,unsigned int property_id) override;

	Project();
//	Project(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx);
	void WhenRotate();
	void WhenTranslate();
	void Animate() override;
	void ScaleAllShapes(float amt, int viewportIndx);
    void SetRenderer(Renderer *renderer);
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
    std::shared_ptr<SceneShape> AddGlobalShape(std::string name, shapes shapeType, std::shared_ptr<ObjectMover> mover,
                                               std::shared_ptr<Layer> layer, std::string shader);
    std::shared_ptr<SceneShape> GetGlobalShape(const std::string& name);
    bool AddGlobalShader(std::shared_ptr<SceneShader> shader);
    std::shared_ptr<SceneShader> GetShader(const std::string& shaderName);
    std::shared_ptr<SceneShader> GetShader(const int shaderId);
private:
    std::vector<std::shared_ptr<SceneShape>> shapesGlobal;
    Renderer *renderer = nullptr;
    long globalTime;
    WindowLocation windowLocation;

    AnimationStatus animationStatus;
    std::map<std::string, std::shared_ptr<SceneShader>> createdShadersByName;
    std::map<int, std::shared_ptr<SceneShader>> createdShadersById;
    std::vector<std::string> allShaders;
    void RefreshShadersList();
    long lastFileSystemRefreshingTimeSeconds;
};


