//
// Created by ditto on 29/06/2022.
//
#include "igl/opengl/glfw/imgui/ImGuiMenu.h"
#include <igl/project.h>

#ifndef ASSIGNMENT1_CPP_PROJECTIMGUIMENU_H
#define ASSIGNMENT1_CPP_PROJECTIMGUIMENU_H

enum Theme {DARK, LIGHT};

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


class ProjectImGuiMenu : public igl::opengl::glfw::imgui::ImGuiMenu {
public:
    ProjectImGuiMenu();
    IGL_INLINE virtual void draw_viewer_menu(igl::opengl::glfw::Viewer* viewer, std::vector<igl::opengl::Camera*> &camera,Eigen::Vector4i& viewWindow,std::vector<DrawInfo *> drawInfos) override;
    ~ProjectImGuiMenu();
private:
//    Icon playButton;
    Theme theme;

};



#endif //ASSIGNMENT1_CPP_PROJECTIMGUIMENU_H
