#pragma once   //maybe should be static class
#include "igl/opengl/glfw/Display.h"
#include "igl/opengl/glfw/renderer.h"
#include "Assignment1.h"
#include "imgui/imgui.h"

	void glfw_mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{	
		if (action == GLFW_PRESS)
		{
			Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
			Assignment1* scn = (Assignment1*)rndr->GetScene();
			double x2, y2;
			
			glfwGetCursorPos(window, &x2, &y2);
			rndr->UpdatePress(x2, y2);
			if (rndr->Picking((int)x2, (int)y2))
			{
				rndr->UpdatePosition(x2, y2);
				if(button == GLFW_MOUSE_BUTTON_LEFT)
					rndr->Pressed();
			}
			else
			{
				rndr->UnPick(2);
			}
		
		}
		else
		{
			Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
			rndr->UnPick(2);
		}
	}
	
	void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Assignment1* scn = (Assignment1*)rndr->GetScene();
        if(yoffset < 0)
            scn->zoom_in();
        else
            scn->zoom_out();
	}
	
	void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Assignment1* scn = (Assignment1*)rndr->GetScene();
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            scn->update_mouse_on_click((float)xpos, (float)ypos);
        else
            scn->update_mouse_with_no_click((float)xpos, (float)ypos);
	}

	void glfw_window_size_callback(GLFWwindow* window, int width, int height)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
        Assignment1* scn = (Assignment1*)rndr->GetScene();

        rndr->resize(window,width,height);
        scn->resize(width, height);
		
	}
	
	void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Assignment1* scn = (Assignment1*)rndr->GetScene();
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			switch (key)
			{
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                    break;
                case GLFW_KEY_UP:
                    scn->increase_chosen_coefficient();
                    break;
                case GLFW_KEY_DOWN:
                    scn->decrease_chosen_coefficient();
                    break;
                case GLFW_KEY_LEFT:
                    scn->increment_IterationNum();
                    break;
                case GLFW_KEY_RIGHT:
                    scn->decrement_IterationNum();
                    break;
                case GLFW_KEY_A:
                    scn->move_horizontally(-0.15f);
                    break;
                case GLFW_KEY_D:
                    scn->move_horizontally(0.15f);
                    break;
                case GLFW_KEY_S:
                    scn->move_vertically(-0.15f);
                    break;
                case GLFW_KEY_W:
                    scn->move_vertically(0.15f);
                    break;
                case GLFW_KEY_1:
                    scn->choose_coefficient(Coefficient::A);
                    break;
                case GLFW_KEY_2:
                    scn->choose_coefficient(Coefficient::B);
                    break;
                 case GLFW_KEY_3:
                     scn->choose_coefficient(Coefficient::C);
                     break;
                 case GLFW_KEY_4:
                     scn->choose_coefficient(Coefficient::D);
                     break;
                default:
                    break;
			}
		}
	}


void Init(Display& display, igl::opengl::glfw::imgui::ImGuiMenu *menu)
{
    display.AddKeyCallBack(glfw_key_callback);
    display.AddMouseCallBacks(glfw_mouse_callback, glfw_scroll_callback, glfw_cursor_position_callback);
    display.AddResizeCallBack(glfw_window_size_callback);
    if(menu != nullptr)
        menu->init(&display);
}
