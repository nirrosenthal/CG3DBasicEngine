#pragma once   //maybe should be static class
#include "igl/opengl/glfw/Display.h"
#include "igl/opengl/glfw/renderer.h"
#include "Project.h"
#include "imgui/imgui.h"



bool holdsLeft;
double xStart, yStart;

float normelize(float num, int maxSize){
    return (((2*num)/maxSize) - 1);
}

bool  inside(float xStart,float yStart,float xEnd,float yEnd,float screenX,float screenY){
    if (xStart > xEnd){
        float tmp = xStart;
        xStart = xEnd;
        xEnd = tmp;
    }
    if (yStart > yEnd){
        float tmp = yStart;
        yStart = yEnd;
        yEnd = tmp;
    }
    bool goodX = xStart <= screenX && screenX<=xEnd;
    bool goodY = yStart <= screenY && screenY <= yEnd;
    return goodX && goodY;
}
//TODO check projection in point Mult scale/ratio
void handlePicking(double xStart, double yStart, double xEnd, double yEnd, Project *scn, Renderer* rndr) {
    xStart = normelize(xStart,scn->VP_Width);
    xEnd = normelize(xEnd,scn->VP_Width);
    yStart = normelize(yStart, scn->VP_Height);
    yEnd= normelize(yEnd, scn->VP_Height);
    for (auto shape: scn->pickedShapes) {
        scn->SetShapeViewport(shape, -3);
    }
    scn->pickedShapes.clear();
    Eigen::Matrix4f projection = rndr->cameras[0]->_projection;
    for (auto shape: scn->getAllShapes()) {
        if (shape->index != 0 && shape->index != 4){
            Eigen::Vector3f pos = shape->getPosition(0);
            Eigen::Vector4f posVec = Eigen::Vector4f(pos.x(), pos.y(),pos.z(),1);
            Eigen::Vector4f res =  projection * posVec;
            float screenX, screenY;
            screenX = res.x();
            screenY = res.y();
            if(inside(xStart,yStart,xEnd,yEnd,screenX,screenY)){
                scn->SetShapeViewport(shape->index, 2);
                scn->pickedShapes.push_back(shape->index);
            }
        }
    }

}



	void glfw_mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{
        Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
        Project* scn = (Project*)rndr->GetScene();
        if(scn->getAnimationStatus() == PLAYING)
            return;



        if (action == GLFW_PRESS)
        {
            Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
            Project* scn = (Project*)rndr->GetScene();
            if (button == GLFW_MOUSE_BUTTON_RIGHT){
                rndr->Pressed();
                glfwGetCursorPos(window, &xStart, &yStart);
            }
            rndr->UpdatePress(xStart, yStart);

        }
        else
        {
            Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
            Project* scn = (Project*)rndr->GetScene();
            if (button == GLFW_MOUSE_BUTTON_RIGHT){
                double xEnd, yEnd;
                glfwGetCursorPos(window, &xEnd, &yEnd);
                rndr->Pressed();
            }
        }
	}
	
	void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Project* scn = (Project*)rndr->GetScene();
        WindowLocation windowLoc = scn->GetWindowLocation();
        auto mouse = scn->GetMouse();
        float xpos = mouse[0];
        float ypos = mouse[1];
        if((xpos >= windowLoc.topLeft.x && xpos <= windowLoc.bottomRight.x
            && ypos >= windowLoc.topLeft.y && ypos <= windowLoc.bottomRight.y) ||
                scn->getAnimationStatus() == PLAYING) {
            return;
        }



        if (rndr->IsPicked())
        {
            rndr->UpdateZpos((int)yoffset);
            rndr->MouseProccessing(GLFW_MOUSE_BUTTON_MIDDLE, 0, scn->GetConrolledCameraId());
        }
        else
        {
            rndr->MoveCamera(scn->GetConrolledCameraId(), rndr->zTranslate, -(float)yoffset);
        }


    }
	
	void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Project* scn = (Project*)rndr->GetScene();
        WindowLocation windowLoc = scn->GetWindowLocation();
        scn->UpdateMouse((float)xpos, (float)ypos);
        if((xpos >= windowLoc.topLeft.x && xpos <= windowLoc.bottomRight.x
            && ypos >= windowLoc.topLeft.y && ypos <= windowLoc.bottomRight.y) ||
            scn->getAnimationStatus() == PLAYING)
            return;


        double xStart, yStart;
		rndr->UpdatePosition((float)xpos,-(float)ypos); // tricking the engine for intuitive mouse movement
        if (rndr->CheckViewport(xpos,ypos, scn->GetConrolledCameraId()))
        {
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            {
                glfwGetCursorPos(window, &xStart, &yStart);
                if (!rndr->IsPressed()){
                    rndr->Pressed();
                }
                if(!scn->IsMousePressed())
                    scn->RightClick(xStart, yStart);

            }
            else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                glfwGetCursorPos(window, &xStart, &yStart);
                scn->LeftClick(xStart, yStart);
                rndr->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT, 0, scn->GetConrolledCameraId());
            }
            else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && rndr->IsPressed() ){
                double xEnd, yEnd;
                glfwGetCursorPos(window, &xEnd, &yEnd);
                handlePicking(xStart, yStart, xEnd, yEnd, scn, rndr);
                rndr->Pressed();
                scn->UnpressMouse(xEnd, yEnd);

            }
            else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && rndr->IsPressed() ){
                double xEnd, yEnd;
                glfwGetCursorPos(window, &xEnd, &yEnd);
                scn->UnpressMouse(xEnd, yEnd);
            }

        }
        auto &io = ImGui::GetIO();
//        auto &io = ImGui::GetIO();
        if(scn->IsMousePressed() && !io.MouseDown[0] && !io.MouseDown[1]) {
            double xEnd, yEnd;
            glfwGetCursorPos(window, &xEnd, &yEnd);
            scn->UnpressMouse(xEnd, yEnd);
        }
	}

	void glfw_window_size_callback(GLFWwindow* window, int width, int height)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
        Project* scn = (Project*)rndr->GetScene();
        rndr->resize(window,width,height);
        scn->UpdateResolution(width, height);
		
	}

    bool escPressed = false;
	void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Project* scn = (Project*)rndr->GetScene();
        auto pickedShapes = scn->GetPickedShapes();
        std::vector<std::shared_ptr<ObjectMover>> pickedMovers;
        for(auto shp : pickedShapes) {
            if(std::find(pickedMovers.begin(), pickedMovers.end(), shp->mover) == pickedMovers.end())
                pickedMovers.push_back(shp->mover);
        }
        if(scn->getAnimationStatus() == PLAYING)
            return;
		//rndr->FreeShapes(2);
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
            if (key != GLFW_KEY_ESCAPE)
                escPressed = false;
			switch (key)
			{
			case GLFW_KEY_ESCAPE:
                if (escPressed)
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
				else
                    escPressed = true;
                break;

//			case GLFW_KEY_SPACE:
//				if (scn->IsActive())
//					scn->Deactivate();
//				else
//					scn->Activate();
//				break;
            case GLFW_KEY_LEFT_CONTROL:
                scn->ChangeControlledCamera();
                break;
			case GLFW_KEY_UP:
                if(pickedShapes.empty())
				    rndr->MoveCamera(scn->GetConrolledCameraId(), scn->xRotate, 0.05f);
                else
                    for(auto mover : pickedMovers) {
                        mover->shift(Eigen::Vector3f(0, 1, 0));
                    }
				break;
			case GLFW_KEY_DOWN:
				//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
				//cout<< "down: "<<endl;
                if(pickedShapes.empty())
				    rndr->MoveCamera(scn->GetConrolledCameraId(), scn->xRotate, -0.05f);
                else {
                    for(auto mover : pickedMovers) {
                        mover->shift(Eigen::Vector3f(0, -1, 0));
                    }
                }
				break;
			case GLFW_KEY_LEFT:
                if(pickedShapes.empty())
				    rndr->MoveCamera(scn->GetConrolledCameraId(), scn->yRotate, 0.05f);
                else {
                    for(auto mover : pickedMovers) {
                        mover->shift(Eigen::Vector3f(-1, 0, 0));
                    }
                }
				break;
			case GLFW_KEY_RIGHT:
				//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
				//cout<< "down: "<<endl;
                if(pickedShapes.empty())
				    rndr->MoveCamera(scn->GetConrolledCameraId(), scn->yRotate, -0.05f);
                else {
                    for(auto mover : pickedMovers) {
                        mover->shift(Eigen::Vector3f(1, 0, 0));
                    }
                }
				break;
            case GLFW_KEY_KP_ADD:
            case GLFW_KEY_EQUAL:
                if(pickedShapes.empty()) { }
                else {
                    for(auto mover : pickedMovers) {
                        mover->shift(Eigen::Vector3f(0, 0, 1));
                    }
                }
                break;
            case GLFW_KEY_KP_SUBTRACT:
            case GLFW_KEY_MINUS:
                if(pickedShapes.empty()) {}
                else {
                        for(auto mover : pickedMovers) {
                            mover->shift(Eigen::Vector3f(0, 0, -1));
                        }
                    }
                break;
//			case GLFW_KEY_U:
//				rndr->MoveCamera(0, scn->yTranslate, 0.25f);
//				break;
//			case GLFW_KEY_D:
//				rndr->MoveCamera(0, scn->yTranslate, -0.25f);
//				break;
//			case GLFW_KEY_L:
//				rndr->MoveCamera(0, scn->xTranslate, -0.25f);
//				break;
//
//			case GLFW_KEY_R:
//				rndr->MoveCamera(0, scn->xTranslate, 0.25f);
//				break;
//
//			case GLFW_KEY_B:
//				rndr->MoveCamera(0, scn->zTranslate, 0.5f);
//				break;
//			case GLFW_KEY_F:
//				rndr->MoveCamera(0, scn->zTranslate, -0.5f);
//				break;
//			case GLFW_KEY_1:
//				std::cout << "picked 1\n";
//				scn->selected_data_index = 1;
//				break;
//			case GLFW_KEY_2:
//				std::cout << "picked 2\n";
//				scn->selected_data_index = 2;
//				break;
//			case GLFW_KEY_3:
//				std::cout << "picked 3\n";
//				scn->selected_data_index = 3;
//				break;
//			default:
//				break;

			}
		}
	}


void Init(Display& display, igl::opengl::glfw::imgui::ImGuiMenu *menu)
{
    display.AddKeyCallBack(glfw_key_callback);
    display.AddMouseCallBacks(glfw_mouse_callback, glfw_scroll_callback, glfw_cursor_position_callback);
    display.AddResizeCallBack(glfw_window_size_callback);
    menu->init(&display);
}
