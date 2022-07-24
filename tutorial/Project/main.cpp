#include "igl/opengl/glfw/renderer.h"
#include "Project.h"
#include "InputManager.h"
#include "Gui.h"

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 1200;
	const int DISPLAY_HEIGHT = 800;
	const float CAMERA_ANGLE = 45.0f;
	const float NEAR = 1.0f;
	const float FAR = 20000.0f;
	const int infoIndx = 2; 
	std::list<int> x, y;
	x.push_back(DISPLAY_WIDTH-1);
	x.push_back(DISPLAY_WIDTH);
	y.push_back(DISPLAY_HEIGHT-1);
	y.push_back(DISPLAY_HEIGHT);
    Display disp = Display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "Animattan");
    Gui *menu = new Gui();

    Renderer* rndr = new Renderer(CAMERA_ANGLE, (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, NEAR, FAR);
    rndr->AddCamera(Eigen::Vector3d(0,0,0), 60, (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, NEAR, FAR, 1);
    rndr->AddCamera(Eigen::Vector3d(0,0,0), 60, (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, NEAR, FAR, 2);
    rndr->AddCamera(Eigen::Vector3d(0,0,0), 60, (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, NEAR, FAR, 3);
	Project *scn = new Project(menu);  //initializing scene
    scn->SetViewportWidth(DISPLAY_WIDTH);
    scn->SetViewportHeight(DISPLAY_HEIGHT);

    Init(disp,menu); //adding callback functions
	scn->Init((float)DISPLAY_WIDTH, (float)DISPLAY_HEIGHT);    //adding shaders, textures, shapes to scene
    rndr->Init(scn,x,y,1, (igl::opengl::glfw::imgui::ImGuiMenu*)menu); // adding scene and viewports to the renderer
    rndr->AddDraw(2 , 0, 3, 0,   rndr->blend | rndr->scaleAbit | rndr-> depthTest | rndr->onPicking);

    disp.SetRenderer(rndr);
    scn->SetRenderer(rndr);
    scn->SetDisplay(&disp);

    disp.launch_rendering(rndr);

	delete scn;
	delete menu;
	
	return 0;
}

