#include "igl/opengl/glfw/renderer.h"
#include "Assignment1.h"
#include "InputManager.h"

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 1200;
	const int DISPLAY_HEIGHT = 800;
	const float CAMERA_ANGLE = 0.0f;
	const float NEAR = 0.0f;
	const float FAR = 120.0f;
	const int infoIndx = 2;
    const float WIDTH_RATIO = 1.0;
	std::list<int> x, y;
	x.push_back(DISPLAY_WIDTH*WIDTH_RATIO);
	x.push_back(DISPLAY_WIDTH);
	y.push_back(DISPLAY_HEIGHT);
    Display disp = Display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OPENGL");
    igl::opengl::glfw::imgui::ImGuiMenu* menu = new igl::opengl::glfw::imgui::ImGuiMenu();
    Renderer* rndr = new Renderer(CAMERA_ANGLE, (float)DISPLAY_WIDTH*WIDTH_RATIO/(float)DISPLAY_HEIGHT, NEAR, FAR);
	Assignment1 *scn = new Assignment1((int)(DISPLAY_WIDTH*WIDTH_RATIO), DISPLAY_HEIGHT);  //initializing scene
	rndr->MoveCamera(0, scn->scaleAll, 0.1f);
    Init(disp,menu); //adding callback functions
	scn->Init();    //adding shaders, textures, shapes to scene
    rndr->Init(scn,x,y,1,menu); // adding scene and viewports to the renderer
    disp.SetRenderer(rndr);

    disp.launch_rendering(rndr);

	delete scn;
	delete rndr;
	delete menu;
	
	return 0;
}

