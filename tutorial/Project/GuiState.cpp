//
// Created by ditto on 01/07/2022.
//

#include "GuiState.h"
#include "StyleManager.h"
#include "MenuManager.h"

NextState::NextState(GuiStep step): step(step), state(nullptr){}
NextState::NextState(GuiStep step, std::shared_ptr<GuiState> state): step(step), state(state) {}

// {Axis, xCylinder,yCylinder,zCylinder, Plane, Cube, Octahedron, Tethrahedron, LineCopy, MeshCopy, Sphere }
GuiState::GuiState(GuiStatus tag): tag(tag) {

}


MenuState::MenuState(float hidpi_scaling_, float pixel_ratio_):
 GuiState(MENU),
 hidpi_scaling_(hidpi_scaling_),
 pixel_ratio_(pixel_ratio_)
 {
 }

NextState MenuState::Run(Project *project, std::vector<igl::opengl::Camera *> &camera, Eigen::Vector4i &viewWindow,
                         std::vector<DrawInfo *> drawInfos, ImFont* font, ImFont *boldFont) {
    bool* p_open = NULL;
    static bool no_titlebar = false;
    static bool no_scrollbar = false;
    static bool no_menu = true;
    static bool no_move = false;
    static bool no_resize = false;
    static bool no_collapse = false;
    static bool no_close = false;
    static bool no_nav = false;
    static bool no_background = false;
    static bool no_bring_to_front = false;

    NextState nextState = NextState(CONTINUE);

    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
    if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
    if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
    if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
    if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;


    ImGui::PushFont(font);
    ImGui::Begin(
            "## MAIN WINDOW", p_open,
            window_flags
    );
    setTheme();
    ImGui::SetWindowPos(ImVec2((float)0, (float)0), ImGuiCond_Always);
    ImGui::SetWindowSize(ImVec2((float)0, (float)0), ImGuiCond_Always);
    ImGui::End();
    no_move = true;
    no_resize = true;
    ImGui::Begin(
            "## MAIN WINDOW", p_open,
            window_flags
    );
    // Theme
    if (ImGui::CollapsingHeader("Theme", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::BeginCombo("##Theme combo", THEMES_REV[theme].c_str())) {
            for (const auto& option : THEMES) {
                std::string name = option.first;
                Theme themeOption = option.second;
                bool is_selected = themeOption == theme;
                if (ImGui::Selectable(name.c_str(), is_selected)) {
                    theme = themeOption;
                }
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }


    }

    // Mesh
    if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
    {
        float w = ImGui::GetContentRegionAvailWidth();
        float p = ImGui::GetStyle().FramePadding.x;
        if (ImGui::Button("Load##Mesh", ImVec2((w-p)/2.f, 0)))
        {
            int savedIndx = project->selected_data_index;
            project->open_dialog_load_mesh();
            if (project->data_list.size() > project->parents.size())
            {
                project->parents.push_back(-1);
                ((ProjectViewerData *)project->data_list.back())->set_visible(false, 1);
                ((ProjectViewerData *)project->data_list.back())->set_visible(true, 2);
                project->data_list.back()->show_faces = 3;
                project->selected_data_index = savedIndx;
            }
        }
        ImGui::SameLine(0, p);
        if (ImGui::Button("Save##Mesh", ImVec2((w-p)/2.f, 0)))
        {
            project->open_dialog_save_mesh();
        }
    }

    // Viewing options
    if (ImGui::CollapsingHeader("Viewing Options", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::Button("Center object", ImVec2(-1, 0)))
        {
            std::cout << "not implemented yet" << std::endl;
//      core[1].align_camera_center(viewer->data().V, viewer->data().F); TODO: add function like this to camera
        }
        //if (ImGui::Button("Snap canonical view", ImVec2(-1, 0)))
        //{
        //  core[1].snap_to_canonical_quaternion();
        //}

        // Zoom
        ImGui::PushItemWidth(80 * menu_scaling());
        if (camera[0]->_ortho)
            ImGui::DragFloat("Zoom", &(camera[0]->_length), 0.05f, 0.1f, 20.0f);
        else
            ImGui::DragFloat("Fov", &(camera[0]->_fov), 0.05f, 30.0f, 90.0f);

        // Select rotation type
        static Eigen::Quaternionf trackball_angle = Eigen::Quaternionf::Identity();
        static bool orthographic = true;

        // Orthographic view
        ImGui::Checkbox("Orthographic view", &(camera[0]->_ortho));
        if (camera[0]->_ortho) {
            camera[0]->SetProjection(0,camera[0]->_relationWH);
        }
        else {
            camera[0]->SetProjection(camera[0]->_fov > 0 ? camera[0]->_fov : 45,camera[0]->_relationWH);
        }

        ImGui::PopItemWidth();
    }

    // Helper for setting viewport specific mesh options
    auto make_checkbox = [&](const char *label, unsigned int &option)
    {
        return ImGui::Checkbox(label,
                               [&]() { return drawInfos[1]->is_set(option); },
                               [&](bool value) { return drawInfos[1]->set(option, value); }
        );
    };
    if (ImGui::CollapsingHeader("Shapes", ImGuiTreeNodeFlags_DefaultOpen)){
        for(const auto& shp : project->getAllShapes()) {
            if(ImGui::Button(shp->name.c_str()))
                nextState = NextState(NEW, std::make_shared<ShapeEditingState>(shp, project->GetShader(shp->shader)));
        }
        if(ImGui::Button("Create a new shape## new shape")){
            nextState = NextState(NEW, std::make_shared<ShapeEditingState>());
        }
    }
    auto layers = project->layerManager.layers;
    if (ImGui::CollapsingHeader("Layers", ImGuiTreeNodeFlags_DefaultOpen)) {
        for (auto const &layerEntry: layers)
        {
            std::string layerName = layerEntry.first;
            auto layer = layerEntry.second;
            std::string buttonLabel = "X##" + layerName;
            if(ImGui::Button(buttonLabel.c_str())){
                if(!project->layerManager.removeLayer(layer)) {
                    std::string errorMsg = "Failed to remove layer " + layerName + ".\r\nPlease make sure it has no shapes and try again.";
                    nextState = NextState(NEW, std::make_shared<ErrorMsgState>(errorMsg));
                }
            }
            ImGui::SameLine();
            bool isShown = !layer->isHidden();
            if(ImGui::Checkbox(layerName.c_str(), &isShown)){
                layer->changeHidden(!isShown);
            }
        }
        ImGui::InputText("##NEW LAYER NAME", newLayerName, 30);
        ImGui::SameLine();
        if(ImGui::Button("Add")){
            auto newName = std::string(newLayerName);
            if(!newName.empty()) {
                project->layerManager.addLayer(newName);
                //delete[] newLayerName;
                newLayerName = strdup("");
            }
        }
    }

    std::string tempStr = "material";
    const char* current_material_item = tempStr.c_str();
    if (ImGui::BeginCombo("##materials combo", current_material_item))
    {
        for (size_t n = 0; n < project->materials.size(); n++)
        {
            bool is_selected = strcmp(current_material_item ,("material" + (std::to_string(n))).c_str())==0;
            std::cout<<"compare: "<<std::string(current_material_item)<<" with "<<("material" + (std::to_string(n))).c_str()<<std::endl;
            if (ImGui::Selectable(("material" + (std::to_string(n))).c_str(), is_selected))
            {
                current_material_item = ("material" + (std::to_string(n))).c_str();
                ((ProjectViewerData *)project->data())->material_indx = n;
                MenuManager::OnSelectMaterial(n);
            }
            if (is_selected){
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    if (ImGui::CollapsingHeader("Camera Split", ImGuiTreeNodeFlags_DefaultOpen)) {
        if(ImGui::RadioButton("No Split",&(((ProjectViewerData *)project->data())->camera_split),0)){
            MenuManager::OnCameraSplitChange(MenuManager::CameraSplitMode::no_split);
        }
        if(ImGui::RadioButton("Split x",&(((ProjectViewerData *)project->data())->camera_split),1)){
            MenuManager::OnCameraSplitChange(MenuManager::CameraSplitMode::split_x);
        }
        if(ImGui::RadioButton("Split y",&(((ProjectViewerData *)project->data())->camera_split),2)){
            MenuManager::OnCameraSplitChange(MenuManager::CameraSplitMode::split_y);
        }

        ImGui::Text("Select Camera for screen 1:");
        const char* current_item = ((ProjectViewerData *)project->data())->cameras.at(((ProjectViewerData *)project->data())->cameraScreen1Indx).c_str();
        if (ImGui::BeginCombo("##cameras combo 1", current_item))
        {
            for (size_t n = 0; n < ((ProjectViewerData *)project->data())->cameras.size(); n++)
            {
                bool is_selected = (current_item == ((ProjectViewerData *)project->data())->cameras.at(n).c_str());
                if (ImGui::Selectable(((ProjectViewerData *)project->data())->cameras.at(n).c_str(), is_selected))
                {
                    current_item = ((ProjectViewerData *)project->data())->cameras.at(n).c_str();
                    ((ProjectViewerData *)project->data())->cameraScreen1Indx = n;
                    std::cout<<"Selected in combo: "<<current_item<<std::endl;
                }
                if (is_selected){
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }
        ImGui::Text("Select Camera for screen 2:");
        const char* current_item2 = (((ProjectViewerData *)project->data())->cameras.at(((ProjectViewerData *)project->data())->cameraScreen2Indx)).c_str();

        if (ImGui::BeginCombo("##cameras combo 2", current_item2))
        {
            for (size_t n = 0; n < ((ProjectViewerData *)project->data())->cameras.size(); n++)
            {
                bool is_selected = (current_item2 == ((ProjectViewerData *)project->data())->cameras.at(n).c_str());
                if (ImGui::Selectable(((ProjectViewerData *)project->data())->cameras.at(n).c_str(), is_selected))
                {
                    current_item2 = ((ProjectViewerData *)project->data())->cameras.at(n).c_str();
                    ((ProjectViewerData *)project->data())->cameraScreen2Indx = n;
                    std::cout<<"Selected in combo: "<<current_item2<<std::endl;
                }
                if (is_selected){
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::Text("Camera name:");
        ImGui::InputText("##@@@.",(((ProjectViewerData *)project->data())->camera_name));
        if(ImGui::Button("Add Camera")){
            ((ProjectViewerData *)project->data())->cameras.push_back(((ProjectViewerData *)project->data())->camera_name);
            MenuManager::OnAddCamera(((ProjectViewerData *)project->data())->camera_name);
        }
    }

    if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
    {
//        ImGui::Image((void*)(intptr_t)(playButton.getTexture()),
//                     ImVec2(playButton.getWidth(), playButton.getHeight()));
        switch(project->getAnimationStatus()) {
            case PLAYING:
                if(ImGui::Button("Pause")){
                    project->Pause();
                }
                ImGui::SameLine();
                if(ImGui::Button("Stop")){ //                 if(ImGui::Button("Stop", ImVec2(60, 30)){
                    project->Stop();
                }
                ImGui::SameLine();
                if(ImGui::Button("Replay")){
                    project->Replay();
                }
                break;
            case PAUSED:
//                if(ImGui::ImageButton((void*)(intptr_t)(playButton.getTexture()),
//                                      ImVec2(playButton.getWidth(), playButton.getHeight()))) {
//                    ((Project*)viewer)->Play();
//                }
                if(ImGui::Button("Play")){
                    project->Play();
                }
                ImGui::SameLine();
                if(ImGui::Button("Stop")){
                    project->Stop();
                }
                ImGui::SameLine();
                if(ImGui::Button("Replay")){
                    project->Replay();
                }
                break;
            case STOPPED:
                if(ImGui::Button("Play")){
                    project->Play();
                }
                break;
        }

        // Adding to viewer time param
        if (ImGui::CollapsingHeader("Slider", ImGuiTreeNodeFlags_DefaultOpen))
        {
            float time = (float)project->GetGlobalTime();
            float maxTime = project->maxTime();
            ImGui::DragFloat("End time", &maxTime, 0.05f, 0.0f, 100.0f, "%.0f");
            if (ImGui::SliderFloat("##Time Scale", &time, 0, maxTime, "%.1f"))
            {
                project->SetGlobalTime((long)time);
            }
        }
    }

    if((ImGui::IsKeyPressed(GLFW_KEY_RIGHT_CONTROL) || ImGui::IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) &&
               ImGui::IsKeyPressed(GLFW_KEY_N))
        nextState = NextState(NEW, std::make_shared<ShapeEditingState>());


    project->UpdateWindowLocation(ImVec2(0,0), ImGui::GetWindowSize());
    ImGui::End();
    ImGui::PopFont();

    return nextState;
}

MenuState::~MenuState() {
    //delete newLayerName;
}

ErrorMsgState::ErrorMsgState(std::string errorMessage): GuiState(ERROR), errorMessage(errorMessage) {}

NextState ErrorMsgState::Run(Project *project, std::vector<igl::opengl::Camera *> &camera, Eigen::Vector4i &viewWindow,
                             std::vector<DrawInfo *> drawInfos, ImFont* font, ImFont *boldFont) {
    NextState nextState(CONTINUE);
    BeginCentered("Error");
    Header("Error", boldFont);
    ImGui::Text("%s", errorMessage.c_str());
    //ImGuiIO& io = ImGui::GetIO();
    //ImVec2 pos(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);

    if(ButtonCenteredOnLine("Close"))
        nextState = NextState(EXIT);

    ImVec2 topLeft = ImGui::GetWindowPos();
    ImVec2 winSize = ImGui::GetWindowSize();
    ImVec2 bottomRight(topLeft.x + winSize.x, topLeft.y + winSize.y);
    project->UpdateWindowLocation(topLeft, bottomRight);

    ImGui::End();
    return nextState;
}

ShapeEditingState::ShapeEditingState():
 GuiState(SHAPE_EDITING),
 editingMode(CREATE_NEW),
 name(strdup("")),
 type(nullptr),
 shader(-1)
{
}

ShapeEditingState::ShapeEditingState(std::shared_ptr<SceneShape> shp, std::shared_ptr<SceneShader> scnShader):
        GuiState(SHAPE_EDITING),
        editingMode(EDIT_EXISTING),
        name(strdup(shp->name.c_str())),
        type(std::make_shared<igl::opengl::glfw::Viewer::shapes>(shp->type)),
        layer(shp->getLayer()),
        shader(shp->shader),
        mover(*(shp->mover.cloneAndCast()))
{
    shaderParams = {};
    if(scnShader == nullptr)
        return;
    for(auto param : scnShader->getParams()) {
        shaderParams.push_back(param->clone());
    }
}


std::map<std::string, igl::opengl::glfw::Viewer::shapes> SHAPE_TYPES = {
        {"Axis", igl::opengl::glfw::Viewer::shapes::Axis},
        {"X Cylinder", igl::opengl::glfw::Viewer::shapes::xCylinder},
        {"Y Cylinder", igl::opengl::glfw::Viewer::shapes::yCylinder},
        {"Z Cylinder", igl::opengl::glfw::Viewer::shapes::zCylinder},
        {"Plane", igl::opengl::glfw::Viewer::shapes::Plane},
        {"Cube", igl::opengl::glfw::Viewer::shapes::Cube},
        {"Octahedron", igl::opengl::glfw::Viewer::shapes::Octahedron},
        {"Tetrahedron", igl::opengl::glfw::Viewer::shapes::Tethrahedron},
        {"LineCopy", igl::opengl::glfw::Viewer::shapes::LineCopy},
        {"MeshCopy", igl::opengl::glfw::Viewer::shapes::MeshCopy},
        {"Sphere", igl::opengl::glfw::Viewer::shapes::Sphere},
};

std::map<igl::opengl::glfw::Viewer::shapes, std::string> SHAPE_TYPES_REV = {
        {igl::opengl::glfw::Viewer::shapes::Axis, "Axis"},
        {igl::opengl::glfw::Viewer::shapes::xCylinder, "X Cylinder"},
        {igl::opengl::glfw::Viewer::shapes::yCylinder, "Y Cylinder"},
        {igl::opengl::glfw::Viewer::shapes::zCylinder, "Z Cylinder"},
        {igl::opengl::glfw::Viewer::shapes::Plane, "Plane"},
        {igl::opengl::glfw::Viewer::shapes::Cube, "Cube"},
        {igl::opengl::glfw::Viewer::shapes::Octahedron, "Octahedron"},
        {igl::opengl::glfw::Viewer::shapes::Tethrahedron, "Tetrahedron"},
        {igl::opengl::glfw::Viewer::shapes::LineCopy, "LineCopy"},
        {igl::opengl::glfw::Viewer::shapes::MeshCopy, "MeshCopy"},
        {igl::opengl::glfw::Viewer::shapes::Sphere, "Sphere"}
};

NextState
ShapeEditingState::Run(Project *project, std::vector<igl::opengl::Camera *> &camera, Eigen::Vector4i &viewWindow,
                       std::vector<DrawInfo *> drawInfos, ImFont *font, ImFont *boldFont) {
    NextState nextState(CONTINUE);

    BeginCentered("Shape Editing");
    std::string header;
    if(editingMode == CREATE_NEW)
        header = "New shape";

    else
        header = "Edit: " + std::string(name);
    Header(header.c_str(), boldFont);
    if (ImGui::CollapsingHeader("Name", ImGuiTreeNodeFlags_DefaultOpen)) {
        if(editingMode == CREATE_NEW)
            ImGui::InputText("##Name", name, 30);
        else
            ImGui::Text("%s", name);
    }
    if (ImGui::CollapsingHeader("Type", ImGuiTreeNodeFlags_DefaultOpen)) {
        std::string firstTypeName = "";
        if(type != nullptr)
            firstTypeName = SHAPE_TYPES_REV[*type];
        if(editingMode == EDIT_EXISTING)
            ImGui::Text("%s", firstTypeName.c_str());
        else {
            if (ImGui::BeginCombo("##Type combo", firstTypeName.c_str())) {
                for (int i = igl::opengl::glfw::Viewer::shapes::Axis;
                     i != igl::opengl::glfw::Viewer::shapes::Sphere; i++) {
                    igl::opengl::glfw::Viewer::shapes currentType = static_cast<igl::opengl::glfw::Viewer::shapes>(i);
                    std::string typeName = SHAPE_TYPES_REV[currentType];
                    bool isSelected = type != nullptr && currentType == *type;
                    if (ImGui::Selectable(typeName.c_str(), isSelected)) {
                        type = std::make_shared<igl::opengl::glfw::Viewer::shapes>(currentType);
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }


                }
                ImGui::EndCombo();

            }
        }

        if (ImGui::CollapsingHeader("Shader", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto allShaders = project->GetAllShaders();
            std::string shaderName = project->GetShaderName(shader);
            if (ImGui::BeginCombo("##Shader combo", shaderName.c_str())) {
                for(auto &currentShader: allShaders) {
                    bool isSelected = currentShader == shaderName;
                    if(ImGui::Selectable(currentShader.c_str(), isSelected)) {
                        shader = project->GetShaderId(currentShader);
                        std::shared_ptr<SceneShader> scnShader = project->GetShader(shader);
                        if(scnShader != nullptr) {
                            shaderParams.clear();
                            for(auto param : scnShader->getParams())
                                shaderParams.push_back(param);
                        }
                    }

                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }
        std::shared_ptr<SceneShader> scnShader = project->GetShader(shader);

        // filtering params for display ONLY
        std::vector<std::shared_ptr<ShaderParam>> paramsForDisplay;
        std::copy_if(shaderParams.begin(), shaderParams.end(), std::back_inserter(paramsForDisplay),
                      // pred:
                      [](const std::shared_ptr<ShaderParam>& param)
                      {return param->isForDisplay();});

        if(!paramsForDisplay.empty()) {
            if (ImGui::CollapsingHeader("Shader Params", ImGuiTreeNodeFlags_DefaultOpen)) {
                for(auto &param : shaderParams) {
                    if(!param->isForDisplay()) {
                        // nothing to display
                        continue;
                    } else if(param->getTag() == INT) {
                        std::string label = param->getName() + " ##PARAM";
                        ImGui::InputInt(label.c_str(), &std::dynamic_pointer_cast<ShaderIntParam>(param)->value);
                    } else if(param->getTag() == FLOAT) {
                        std::string label = param->getName() + " ##PARAM";
                        ImGui::InputFloat(label.c_str(), &std::dynamic_pointer_cast<ShaderFloatParam>(param)->value);
                    } else if(param->getTag() == VEC4_INT) {
                        auto inputVec = std::dynamic_pointer_cast<ShaderIntVec4Param>(param);
                        ImGui::Text("%s", (param->getName() + ":").c_str());
                        ImGui::InputInt(("##PARAM" + param->getName() + "0").c_str(), &inputVec->value[0]);
                        ImGui::SameLine();
                        ImGui::InputInt(("##PARAM" + param->getName() + "1").c_str(), &inputVec->value[1]);
                        ImGui::SameLine();
                        ImGui::InputInt(("##PARAM" + param->getName() + "2").c_str(), &inputVec->value[2]);
                        ImGui::SameLine();
                        ImGui::InputInt(("##PARAM" + param->getName() + "3").c_str(), &inputVec->value[3]);
                    } else if (param->getTag() == VEC4_FLOAT) {
                        auto inputVec = std::dynamic_pointer_cast<ShaderFloatVec4Param>(param);
                        ImGui::Text("%s", (param->getName() + ":").c_str());
                        ImGui::InputFloat(("##PARAM" + param->getName() + "0").c_str(), &inputVec->value[0]);
                        ImGui::SameLine();
                        ImGui::InputFloat(("##PARAM" + param->getName() + "1").c_str(), &inputVec->value[1]);
                        ImGui::SameLine();
                        ImGui::InputFloat(("##PARAM" + param->getName() + "2").c_str(), &inputVec->value[2]);
                        ImGui::SameLine();
                        ImGui::InputFloat(("##PARAM" + param->getName() + "3").c_str(), &inputVec->value[3]);
                    } else if (param->getTag() == MAT4_FLOAT) {
                        auto inputMat = std::dynamic_pointer_cast<ShaderFloatMat4Param>(param);
                        ImGui::Text("%s", (param->getName() + ":").c_str());
                        ImGui::InputFloat(("##PARAM" + param->getName() + "0,0").c_str(), &inputMat->value(0));
                        ImGui::SameLine();
                        ImGui::InputFloat(("##PARAM" + param->getName() + "0,1").c_str(), &inputMat->value(1));
                        ImGui::SameLine();
                        ImGui::InputFloat(("##PARAM" + param->getName() + "0,2").c_str(), &inputMat->value(2));
                        ImGui::SameLine();
                        ImGui::InputFloat(("##PARAM" + param->getName() + "0,3").c_str(), &inputMat->value(3));
                        ImGui::InputFloat(("##PARAM" + param->getName() + "1,0").c_str(), &inputMat->value(4));
                        ImGui::SameLine();
                        ImGui::InputFloat(("##PARAM" + param->getName() + "1,1").c_str(), &inputMat->value(5));
                        ImGui::SameLine();
                        ImGui::InputFloat(("##PARAM" + param->getName() + "1,2").c_str(), &inputMat->value(6));
                        ImGui::SameLine();
                        ImGui::InputFloat(("##PARAM" + param->getName() + "1,3").c_str(), &inputMat->value(7));
                        ImGui::InputFloat(("##PARAM" + param->getName() + "2,0").c_str(), &inputMat->value(8));
                        ImGui::SameLine();
                        ImGui::InputFloat(("##PARAM" + param->getName() + "2,1").c_str(), &inputMat->value(9));
                        ImGui::SameLine();
                        ImGui::InputFloat(("##PARAM" + param->getName() + "2,2").c_str(), &inputMat->value(10));
                        ImGui::SameLine();
                        ImGui::InputFloat(("##PARAM" + param->getName() + "2,3").c_str(), &inputMat->value(11));
                        ImGui::InputFloat(("##PARAM" + param->getName() + "3,0").c_str(), &inputMat->value(12));
                        ImGui::SameLine();
                        ImGui::InputFloat(("##PARAM" + param->getName() + "3,1").c_str(), &inputMat->value(13));
                        ImGui::SameLine();
                        ImGui::InputFloat(("##PARAM" + param->getName() + "3,2").c_str(), &inputMat->value(14));
                        ImGui::SameLine();
                        ImGui::InputFloat(("##PARAM" + param->getName() + "3,3").c_str(), &inputMat->value(15));
                    } else if (param->getTag() == RGB) {
                        auto rgb = std::dynamic_pointer_cast<ShaderRGBParam>(param);
                        ImGui::Text("%s", (param->getName() + ":").c_str());
                        ImGui::ColorPicker4(("##" + param->getName() + "COLOR PICKING").c_str(), rgb->value);
                    }
                }
            }
        }

        if (ImGui::CollapsingHeader("Layer", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto allLayers = project->layerManager.layers;
            std::string layerName;
            if(layer != nullptr)
                layerName = layer->getName();
            if (ImGui::BeginCombo("##Layer combo", layerName.c_str())) {
                for(auto currentLayerEntry: allLayers) {
                    auto currentLayer = currentLayerEntry.second;
                    bool isSelected = currentLayer == layer;

                    if(ImGui::Selectable(currentLayer->getName().c_str(), isSelected)) {
                        layer = currentLayer;
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }

        bool saveSucceed = true;
        if(ImGui::Button("Save")){
            nextState = NextState(EXIT);
            if(std::string(name).empty()) {
                nextState = NextState(NEW, std::make_shared<ErrorMsgState>("Name cannot be empty!"));
                saveSucceed = false;
            }
            if(project->layerManager.getLayer(name) != nullptr) {
                nextState = NextState(NEW, std::make_shared<ErrorMsgState>(
                        "Shape: " + std::string(name) + " already exists!"));
                saveSucceed = false;
            }
            if(layer == nullptr) {
                nextState = NextState(NEW, std::make_shared<ErrorMsgState>("Layer cannot be empty!"));
                saveSucceed = false;
            }
            if(type == nullptr) {
                nextState = NextState(NEW, std::make_shared<ErrorMsgState>("Type cannot be empty!"));
                saveSucceed = false;
            }
            if(editingMode == CREATE_NEW && saveSucceed) {
                std::vector<Eigen::Vector3f> points = {Eigen::Vector3f(0,0,0),
                                                       Eigen::Vector3f(0,20,0),
                                                       Eigen::Vector3f(-10,-10,-100),
                                                       Eigen::Vector3f(0,0,0)};
                std::shared_ptr<ObjectMoverBezier> bez = std::make_shared<ObjectMoverBezier>(points, 0, 500);
                project->AddGlobalShape(name, *type, bez, layer, project->GetShaderName(shader));
            } else if(editingMode == EDIT_EXISTING && saveSucceed) {
                auto shape = project->GetGlobalShape(std::string(name));
                shape->name = name;
                shape->mover = mover;
                shape->shader = shader;
                project->GetShader(shader)->setParams(shaderParams);
                if(layer != shape->getLayer()) {
                    shape->getLayer()->deleteShape(shape);
                    shape->changeLayer(layer);
                    layer->addShape(shape);
                }
            }

        }
        ImGui::SameLine();
        if(ImGui::Button("Close")){
            nextState = NextState(EXIT);
        }
    }

    ImVec2 topLeft = ImGui::GetWindowPos();
    ImVec2 winSize = ImGui::GetWindowSize();
    ImVec2 bottomRight(topLeft.x + winSize.x, topLeft.y + winSize.y);
    project->UpdateWindowLocation(topLeft, bottomRight);
    ImGui::End();
    return nextState;
}

ShapeEditingState::~ShapeEditingState() {
    //delete[] name;
}

