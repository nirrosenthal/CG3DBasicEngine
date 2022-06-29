//
// Created by ditto on 29/06/2022.
//
#include "igl/opengl/glfw/imgui/ImGuiMenu.h"
#include <igl/project.h>

#ifndef ASSIGNMENT1_CPP_PROJECTIMGUIMENU_H
#define ASSIGNMENT1_CPP_PROJECTIMGUIMENU_H


class ProjectImGuiMenu : public igl::opengl::glfw::imgui::ImGuiMenu {
public:
    IGL_INLINE virtual void draw_viewer_menu(igl::opengl::glfw::Viewer* viewer, std::vector<igl::opengl::Camera*> &camera,Eigen::Vector4i& viewWindow,std::vector<DrawInfo *> drawInfos) override;
};


#endif //ASSIGNMENT1_CPP_PROJECTIMGUIMENU_H
