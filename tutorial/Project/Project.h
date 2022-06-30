#pragma once
#include "igl/opengl/glfw/Viewer.h"
#include "SceneShape.h"
#include "igl/opengl/glfw/renderer.h"
#include "ProjectViewerData.h"

enum AnimationStatus {PLAYING, STOPPED, PAUSED};
class Project : public igl::opengl::glfw::Viewer
{
	
public:

    IGL_INLINE ProjectViewerData* data(int mesh_id = -1);
    IGL_INLINE const ProjectViewerData* data(int mesh_id = -1) const;
    IGL_INLINE int append_mesh(bool visible = true);

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
    ImVec2 menuSize;
    float maxTime();
    AnimationStatus getAnimationStatus();
	~Project(void);
    long GetGlobalTime();
    void SetGlobalTime(long time);
    LayerManager layerManager;
private:
    std::vector<std::shared_ptr<SceneShape>> shapesGlobal;
    Renderer *renderer = nullptr;
    long globalTime;

    std::shared_ptr<SceneShape> AddGlobalShape(std::string name, shapes shapeType, std::shared_ptr<ObjectMover> mover,
                   std::shared_ptr<Layer> layer);
    AnimationStatus animationStatus;
};


