//
// Created by ditto on 01/07/2022.
//
#include "Project.h"

#ifndef ASSIGNMENT1_CPP_GUISTATE_H
#define ASSIGNMENT1_CPP_GUISTATE_H
enum GuiStatus{MENU, ERROR};
enum GuiStep{CONTINUE, NEW, EXIT};

class GuiState;
class NextState {
public:
    NextState(GuiStep step);
    NextState(GuiStep step, std::shared_ptr<GuiState> state);
    GuiStep step;
    std::shared_ptr<GuiState> state;
};
class GuiState {
public:
    GuiState(GuiStatus tag);
    GuiStatus getTag() {return tag;};
    virtual NextState Run(Project* project,
                     std::vector<igl::opengl::Camera*> &camera,
                     Eigen::Vector4i& viewWindow,std::vector<DrawInfo *> drawInfos,
                     ImFont* font, ImFont *boldFont)=0;

private:
    GuiStatus tag;
};

class MenuState: public GuiState {
public:
    MenuState(float hidpi_scaling_, float pixel_ratio_);
    NextState Run(Project* project,
                  std::vector<igl::opengl::Camera*> &camera,
                  Eigen::Vector4i& viewWindow,std::vector<DrawInfo *> drawInfos,
                  ImFont* font, ImFont *boldFont);
    float menu_scaling() { return hidpi_scaling_ / pixel_ratio_; }
    ~MenuState();
private:
    float hidpi_scaling_;
    float pixel_ratio_;
    char* newLayerName = strdup("");

};

class ErrorMsgState: public GuiState {
public:
    ErrorMsgState(std::string errorMessage);
    NextState Run(Project* project,
                  std::vector<igl::opengl::Camera*> &camera,
                  Eigen::Vector4i& viewWindow,std::vector<DrawInfo *> drawInfos,
                  ImFont* font, ImFont *boldFont);
private:
    std::string errorMessage;

};


#endif //ASSIGNMENT1_CPP_GUISTATE_H
