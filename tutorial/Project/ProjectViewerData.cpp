//
// Created by ditto on 29/06/2022.
//

#include "ProjectViewerData.h"
IGL_INLINE ProjectViewerData::ProjectViewerData():
    igl::opengl::ViewerData::ViewerData(),
    camera_split      (0),
    is_visible        (~unsigned(0)) {
    cameras.emplace_back("Main Camera");
    layers.emplace_back("Default layer",true);
}

void ProjectViewerData::set_visible(bool value, unsigned int property_id) {
    if (value)
        is_visible |= property_id;
    else
        is_visible &= ~property_id;
}

void ProjectViewerData::clear() {
    ViewerData::clear();
    face_based = false;
}
