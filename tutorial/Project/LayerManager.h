/*

	- need to add the line [ #include "igl/opengl/glfw/Viewer.h" ] to Project.h

*/
#include <string>
#include <memory>
#include <map>
#include <vector>

class SceneShape;
class Layer
{
public:
	Layer(std::string layerName);
	void hide() {hidden = true;};
    void show() {hidden = false;};
    bool isHidden() {return hidden;};
    void changeHidden(bool hide) { hidden = hide; }
	void addShape(std::shared_ptr<SceneShape> shape);
    void deleteShape(std::shared_ptr<SceneShape> shape);
    std::string getName() {return layerName;};
    std::vector<std::shared_ptr<SceneShape>> getShapes() {return shapes;};
	~Layer(void) {}



private:
    bool hidden;
    std::string layerName;
    std::vector<std::shared_ptr<SceneShape>> shapes;
};

class LayerManager 
{
public:
    LayerManager();
    std::shared_ptr<Layer> getLayer(const std::string& name);
	bool addLayer(const std::string& layerName);
	bool removeLayer(std::shared_ptr<Layer> layer);
	~LayerManager(void) {}
	std::map<std::string, std::shared_ptr<Layer>> layers;
};