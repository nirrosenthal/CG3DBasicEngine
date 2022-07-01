//
// Created by ditto on 29/06/2022.
//
#include "igl/opengl/glfw/imgui/ImGuiMenu.h"
#include "imgui.h"
#include "Project.h"
#include "GuiState.h"
#include <igl/project.h>
#include <stack>

#ifndef ASSIGNMENT1_CPP_PROJECTIMGUIMENU_H
#define ASSIGNMENT1_CPP_PROJECTIMGUIMENU_H


class Icon {
public:
    Icon(const char* filename);
    ~Icon();
    GLuint *getTexture();
    int getWidth();
    int getHeight();
private:
    GLuint *texture;
    int width;
    int height;
};




class Gui : public igl::opengl::glfw::imgui::ImGuiMenu {
public:
    Gui();
    IGL_INLINE virtual void init(Display* disp);
    IGL_INLINE virtual void draw_viewer_menu(igl::opengl::glfw::Viewer* viewer, std::vector<igl::opengl::Camera*> &camera,Eigen::Vector4i& viewWindow,std::vector<DrawInfo *> drawInfos) override;
    ~Gui();
private:
//    Icon playButton;
    std::stack<std::shared_ptr<GuiState>> guiStates;
    ImFont* font = nullptr;
    ImFont *boldFont = nullptr;
};



#endif //ASSIGNMENT1_CPP_PROJECTIMGUIMENU_H
