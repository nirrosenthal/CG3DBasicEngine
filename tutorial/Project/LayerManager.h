/*

	- need to add the line [ #include "igl/opengl/glfw/Viewer.h" ] to Project.h

*/
#include <list>
using namespace std;

class Shape
{};
class Layer
{

public:
	Layer(string layerName);
	void changeHidden(bool hide) { hidden = hide; }
	void addShape(Shape shape);
	
	~Layer(void) {}

	bool hidden;
	string layerName;
	list<Shape> shapes;
};

class LayerManager 
{
public:

    LayerManager();
	bool isHidden(string layerName);
	void hideLayer(string layerName);
	void showLayer(string layerName);
	void addShape(string layerName, Shape shape);
	void removeShape(string layerName, Shape shape);
	bool addLayer(string layerName);
	void removeLayer(string layerName);
	~LayerManager(void) {}
	

	list<Layer> layers;
};