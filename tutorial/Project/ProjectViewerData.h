//
// Created by ditto on 29/06/2022.
//

#ifndef ASSIGNMENT1_CPP_PROJECTVIEWERDATA_H
#define ASSIGNMENT1_CPP_PROJECTVIEWERDATA_H
#include "igl/opengl/ViewerData.h"

class ProjectViewerData: public igl::opengl::ViewerData {
public:
    ProjectViewerData();
    IGL_INLINE void set_visible(bool value, unsigned int property_id = 1);
    IGL_INLINE void clear();
    IGL_INLINE void set_camera_split(int camera_split);
    int camera_split;
    int cameraScreen1Indx = 0;
    int cameraScreen2Indx = 0;
    int cameraAnimationIndx = 0;
    std::vector<std::string> cameras;
    std::string camera_name;
    int material_indx = 0;
    unsigned int is_visible;
};


#endif //ASSIGNMENT1_CPP_PROJECTVIEWERDATA_H
