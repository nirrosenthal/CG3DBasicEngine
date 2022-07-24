//
// Created by ditto on 01/07/2022.
//

#include "GuiState.h"
#include "StyleManager.h"

// {Axis, xCylinder,yCylinder,zCylinder, Plane, Cube, Octahedron, Tethrahedron, LineCopy, MeshCopy, Sphere }
GuiState::GuiState(GuiStatus tag): tag(tag) {

}


MenuState::MenuState(float hidpi_scaling_, float pixel_ratio_):
 GuiState(MENU),
 hidpi_scaling_(hidpi_scaling_),
 pixel_ratio_(pixel_ratio_)
 {
 }

std::map<std::string, SplitCameraOption> CAMERA_SPLIT_OPTIONS = {
        {"Unsplit", UNSPLIT},
        {"Split x", SPLITX},
        {"Split y", SPLITY}
};

std::map<SplitCameraOption,std::string> CAMERA_SPLIT_OPTIONS_REV = {
        {UNSPLIT,"Unsplit"},
        {SPLITX,"Split x"},
        {SPLITY,"Split y"}
};

void MenuState::Run(Project *project, std::vector<igl::opengl::Camera *> &camera, Eigen::Vector4i &viewWindow,
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

    if (ImGui::CollapsingHeader("Background Shader", ImGuiTreeNodeFlags_DefaultOpen)) {
        std::string backgroundShader = project->GetBackgroundShader();
        if (ImGui::BeginCombo("##background shader combo", backgroundShader.c_str())) {
            for (const auto& shader : project->GetAllShaders()) {
                bool isSelected = backgroundShader == shader;
                if (ImGui::Selectable(shader.c_str(), isSelected)) {
                    project->SetBackgroundShader(shader);
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
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
            std::string deleteButtonLabel = "X##" + shp->name;
            if(ImGui::Button(deleteButtonLabel.c_str())){
                project->DeleteShape(shp);
            }
            ImGui::SameLine();
            if(ImGui::Button(shp->name.c_str()))
                project->guiStates.push(std::make_shared<ShapeEditingState>(shp, project->GetShader(shp->shader)));

        }
    }
    if(ImGui::Button("Create a new shape## new shape")){
        project->guiStates.push(std::make_shared<ShapeEditingState>());
    }
    if (ImGui::CollapsingHeader("Search For Shaders", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputText("##shader search pattern", shadersSearchPattern, 30);
        if (ImGui::Button("Clear ##shadersSearchPattern"))
            shadersSearchPattern = strdup("");
    }
    if(!std::string(shadersSearchPattern).empty())
        ImGui::SetNextTreeNodeOpen(true);

    auto allShaders = project->GetAllShaders();
    std::vector<std::string> filteredShaders;
    std::copy_if(allShaders.begin(), allShaders.end(), std::back_inserter(filteredShaders),
                 // pred:
                 [this](std::string shaderName){
                    auto capitalShaderName = shaderName;
                    auto capitalPattern = std::string(shadersSearchPattern);
                    std::transform(capitalShaderName.begin(), capitalShaderName.end(),capitalShaderName.begin(), ::toupper);
                    std::transform(capitalPattern.begin(), capitalPattern.end(),capitalPattern.begin(), ::toupper);
                    // shader name starts with pattern
                    return capitalShaderName.rfind(capitalPattern, 0) == 0;
                });

    if(ImGui::CollapsingHeader("Edit Selected Shader")) {
        for (const auto &shader: filteredShaders) {
            if(ImGui::Button((shader + "##SHADER").c_str())) {
                std::shared_ptr <SceneShader> selectedShader = project->GetShader(shader);
                if (selectedShader == nullptr) {
                    project->guiStates.push(std::make_shared<ErrorMsgState>("An unknown error has occurred"));
                } else {
                    project->guiStates.push(std::make_shared<ShaderEditingState>(selectedShader));
                }
            }
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
                    project->guiStates.push(std::make_shared<ErrorMsgState>(errorMsg));
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
    if (ImGui::CollapsingHeader("Movement Curves", ImGuiTreeNodeFlags_DefaultOpen)){
        for(const auto& curve : project->GetAllMovementCurves()) {
            if(ImGui::Button(curve.c_str()))
                project->guiStates.push(std::make_shared<MovementCurveEditingState>(curve, project->GetCurve(curve)));
        }
    }
    if(ImGui::Button("Create a new Curve")){
        project->guiStates.push(std::make_shared<MovementCurveEditingState>());
    }

//    std::string tempStr = "material";
//    const char* current_material_item = tempStr.c_str();
//    if (ImGui::BeginCombo("##materials combo", current_material_item))
//    {
//        for (size_t n = 0; n < project->materials.size(); n++)
//        {
//            bool is_selected = strcmp(current_material_item ,("material" + (std::to_string(n))).c_str())==0;
//            std::cout<<"compare: "<<std::string(current_material_item)<<" with "<<("material" + (std::to_string(n))).c_str()<<std::endl;
//            if (ImGui::Selectable(("material" + (std::to_string(n))).c_str(), is_selected))
//            {
//                current_material_item = ("material" + (std::to_string(n))).c_str();
//                ((ProjectViewerData *)project->data())->material_indx = n;
//                // implement add material
//            }
//            if (is_selected){
//                ImGui::SetItemDefaultFocus();
//            }
//        }
//
//        ImGui::EndCombo();
//    }



        ImGui::Text("Camera split option");

        const std::string splitOption = CAMERA_SPLIT_OPTIONS_REV[project->GetSplitCameraOption()];
        if (ImGui::BeginCombo("##split options combo", splitOption.c_str())) {
            for (auto entry: CAMERA_SPLIT_OPTIONS_REV) {
                SplitCameraOption splitEnum = entry.first;
                std::string splitString = entry.second;
                bool isSelected = splitEnum == project->GetSplitCameraOption();
                if (ImGui::Selectable(splitString.c_str(), isSelected)) {
                    // manually set camera options so that we could save the previous state when entering animation mode
                    switch (splitEnum) {
                        case UNSPLIT:
                            project->Unsplit();
                            break;
                        case SPLITX:
                            project->SplitX();
                            break;
                        case SPLITY:
                            project->SplitY();
                            break;
                    }
                    project->SetPrevSplitCameraOption(project->GetSplitCameraOption());
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
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
            // implement add camera
        }


    if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
    {
//        ImGui::Image((void*)(intptr_t)(playButton.getTexture()),
//                     ImVec2(playButton.getWidth(), playButton.getHeight()));
        switch(project->getAnimationStatus()) {
            case PLAYING:
                throw std::runtime_error("Menu shouldn't be open on animation status PLAYING");
                break;
            case PAUSED:
//                if(ImGui::ImageButton((void*)(intptr_t)(playButton.getTexture()),
//                                      ImVec2(playButton.getWidth(), playButton.getHeight()))) {
//                    ((Project*)viewer)->Play();
//                }
                if(ImGui::Button("Play")){
                    project->guiStates.push(std::make_shared<MediaSliderState>());
                    project->Play();
                }
                ImGui::SameLine();
                if(ImGui::Button("Stop")){
                    project->Stop();
                }
                ImGui::SameLine();
                if(ImGui::Button("Replay")){
                    project->guiStates.push(std::make_shared<MediaSliderState>());
                    project->Replay();
                }
                break;
            case STOPPED:
                if(ImGui::Button("Play")){
                    project->guiStates.push(std::make_shared<MediaSliderState>());
                    project->Play();
                }
                break;
        }
        float time = (float)project->GetGlobalTime();
        float maxTime = project->maxTime();
        if (ImGui::SliderFloat("##Time Scale", &time, 0, maxTime, "%.1f"))
        {
            project->SetGlobalTime((long)time);
        }


    }

    if((ImGui::IsKeyPressed(GLFW_KEY_RIGHT_CONTROL) || ImGui::IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) &&
               ImGui::IsKeyPressed(GLFW_KEY_N))
        project->guiStates.push(std::make_shared<ShapeEditingState>());


    ImGui::PopFont();

}

MenuState::~MenuState() {
    //delete newLayerName;
}

ErrorMsgState::ErrorMsgState(std::string errorMessage): GuiState(ERROR), errorMessage(errorMessage) {}

void ErrorMsgState::Run(Project *project, std::vector<igl::opengl::Camera *> &camera, Eigen::Vector4i &viewWindow,
                             std::vector<DrawInfo *> drawInfos, ImFont* font, ImFont *boldFont) {
    BeginCentered("Error");
    Header("Error", boldFont);
    ImGui::Text("%s", errorMessage.c_str());
    //ImGuiIO& io = ImGui::GetIO();
    //ImVec2 pos(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);

    if(ButtonCenteredOnLine("Close"))
        project->guiStates.pop();

    ImVec2 topLeft = ImGui::GetWindowPos();
    ImVec2 winSize = ImGui::GetWindowSize();
    ImVec2 bottomRight(topLeft.x + winSize.x, topLeft.y + winSize.y);

}

ShapeEditingState::ShapeEditingState():
 GuiState(SHAPE_EDITING),
 editingMode(CREATE_NEW),
 source(HARD_CODED),
 name(strdup("")),
 type(nullptr),
 shader(-1),
 sizePercents(100)
{
}

ShapeEditingState::ShapeEditingState(std::shared_ptr<SceneShape> shp, std::shared_ptr<SceneShader> scnShader):
        GuiState(SHAPE_EDITING),
        editingMode(EDIT_EXISTING),
        source(shp->getSource()),
        name(strdup(shp->name.c_str())),
        file(shp->getFile()),
        type(std::make_shared<igl::opengl::glfw::Viewer::shapes>(shp->type)),
        layer(shp->getLayer()),
        shader(shp->shader),
        mover(shp->mover->cloneAndCast()),
        sizePercents(shp->GetScale())
{
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

void
ShapeEditingState::Run(Project *project, std::vector<igl::opengl::Camera *> &camera, Eigen::Vector4i &viewWindow,
                       std::vector<DrawInfo *> drawInfos, ImFont *font, ImFont *boldFont) {

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
    if (editingMode == CREATE_NEW) {
        if (ImGui::CollapsingHeader("Source", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::RadioButton("From Menu", source == HARD_CODED)) {
                source = HARD_CODED;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("From File", source == FROM_FILE)) {
                source = FROM_FILE;
            }
        }
    }
    if (source == HARD_CODED) {
        if (ImGui::CollapsingHeader("Type", ImGuiTreeNodeFlags_DefaultOpen)) {
            std::string firstTypeName = "";
            if (type != nullptr)
                firstTypeName = SHAPE_TYPES_REV[*type];
            if (editingMode == EDIT_EXISTING)
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
        }
    }

    if (source == FROM_FILE) {
        if (ImGui::CollapsingHeader("File", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto allFiles = project->GetAllShapeFiles();
            if (editingMode == EDIT_EXISTING)
                ImGui::Text("%s", file.c_str());
            else {
                if (ImGui::BeginCombo("##File combo", file.c_str())) {
                    for (auto shapeFile: allFiles) {
                        bool isSelected = file == shapeFile;
                        if (ImGui::Selectable(shapeFile.c_str(), isSelected)) {
                            file = shapeFile;
                        }
                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }
        }
    }
    if(ImGui::CollapsingHeader("Size"), ImGuiTreeNodeFlags_DefaultOpen) {
        ImGui::InputFloat("##Shape Size", &sizePercents);
    }
    if(sizePercents < 0)
        sizePercents = 0;

    if (ImGui::CollapsingHeader("Shader"), ImGuiTreeNodeFlags_DefaultOpen) {
        auto allShaders = project->GetAllShaders();
        std::string shaderName = project->GetShaderName(shader);
        if (ImGui::BeginCombo("##Shader combo", shaderName.c_str())) {
            for(auto &currentShader: allShaders) {
                bool isSelected = currentShader == shaderName;
                if(ImGui::Selectable(currentShader.c_str(), isSelected)) {
                    shader = project->GetShaderId(currentShader);
                    std::shared_ptr<SceneShader> scnShader = project->GetShader(shader);
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
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
    if (ImGui::CollapsingHeader("Movement Curves", ImGuiTreeNodeFlags_DefaultOpen)) {
        auto allCurves = project->GetAllMovementCurves();
        std::string moverName;
        if(mover != nullptr)
            moverName = mover->name;
        if (ImGui::BeginCombo("##Mover combo", moverName.c_str())) {
            for(auto curve : allCurves) {
                bool isSelected = moverName == curve;
                if(ImGui::Selectable(curve.c_str(), isSelected)) {
                    mover = project->GetCurve(curve);
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        bool saveSucceed = true;
        if(ImGui::Button("Save")){
            if(std::string(name).empty()) {
                project->guiStates.push(std::make_shared<ErrorMsgState>("Name cannot be empty!"));
                saveSucceed = false;
            }
            if(project->layerManager.getLayer(name) != nullptr) {
                project->guiStates.push(std::make_shared<ErrorMsgState>(
                        "Shape: " + std::string(name) + " already exists!"));
                saveSucceed = false;
            }
            if(layer == nullptr) {
                project->guiStates.push(std::make_shared<ErrorMsgState>("Layer cannot be empty!"));
                saveSucceed = false;
            }
            if(source == HARD_CODED && type == nullptr) {
                project->guiStates.push(std::make_shared<ErrorMsgState>("Type cannot be empty!"));
                saveSucceed = false;
            }
            if(source == FROM_FILE && file.empty()) {
                project->guiStates.push(std::make_shared<ErrorMsgState>("File cannot be empty!"));
                saveSucceed = false;
            }
            if(mover == nullptr) {
                project->guiStates.push(std::make_shared<ErrorMsgState>("Curve cannot be empty!"));
                saveSucceed = false;
            }
            if(editingMode == CREATE_NEW && saveSucceed) {
                if(source == HARD_CODED)
                    project->AddGlobalShape(name, *type, mover, layer, project->GetShaderName(shader));
                else
                    project->AddGlobalShape(name, file, mover, layer, project->GetShaderName(shader));
            } else if(editingMode == EDIT_EXISTING && saveSucceed) {
                auto shape = project->GetGlobalShape(std::string(name));
                shape->name = name;
                shape->mover = mover;
                shape->shader = shader;
                if(layer != shape->getLayer()) {
                    shape->getLayer()->deleteShape(shape);
                    shape->changeLayer(layer);
                    layer->addShape(shape);
                }

                if(sizePercents != shape->GetScale()) {
                    shape->Rescale(sizePercents);
                }
                project->guiStates.pop();
            }

        }
        ImGui::SameLine();
        if(ImGui::Button("Close")){
           project->guiStates.pop();
        }
    }

}

ShapeEditingState::~ShapeEditingState() {
    //delete[] name;
}

ShaderEditingState::ShaderEditingState(std::shared_ptr<SceneShader> scnShader):
        GuiState(SHADER_EDITING),
        shaderName(scnShader->getName()) {
    for(const auto& param : scnShader->getParams())
        shaderParams.push_back(param->clone());
}

void
ShaderEditingState::Run(Project *project, std::vector<igl::opengl::Camera *> &camera, Eigen::Vector4i &viewWindow,
                        std::vector<DrawInfo *> drawInfos, ImFont *font, ImFont *boldFont) {
    BeginCentered("Editing shader");

    // filtering params for display ONLY
    std::vector<std::shared_ptr<ShaderParam>> paramsForDisplay;
    std::copy_if(shaderParams.begin(), shaderParams.end(), std::back_inserter(paramsForDisplay),
                 // pred:
                 [](const std::shared_ptr<ShaderParam>& param)
                 {return param->isForDisplay();});

    Header(("Editing shader: " + shaderName).c_str(), boldFont);
    if(paramsForDisplay.empty()) {
        ImGui::Text("This shader has no params to edit");
    } else {
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
    if(!paramsForDisplay.empty()) {
        if (ImGui::Button("Save")) {
            project->GetShader(shaderName)->setParams(shaderParams);
            project->guiStates.pop();
        }
        ImGui::SameLine();
    }
    if(ImGui::Button("Close")) {
        project->guiStates.pop();
    }

}

MovementCurveEditingState::MovementCurveEditingState(std::string name, std::shared_ptr<ObjectMoverSplit> mover):
    GuiState(CURVE_EDITING), curveName(strdup(name.c_str())), editingMode(EDIT_EXISTING),
    startTime(mover->getStartTime()){
        for(auto &subMover : mover->movers)
            movers.push_back(std::make_shared<ObjectMoverForGui>(subMover));
    }

MovementCurveEditingState::MovementCurveEditingState(): GuiState(CURVE_EDITING), editingMode(CREATE_NEW),
    startTime(0), curveName(strdup("")) {}

void MovementCurveEditingState::Run(Project *project, std::vector<igl::opengl::Camera *> &camera,
                                         Eigen::Vector4i &viewWindow, std::vector<DrawInfo *> drawInfos, ImFont *font,
                                         ImFont *boldFont) {
    BeginCentered("Editing Curve");
    if(editingMode == CREATE_NEW) {
        Header("New Curve", boldFont);
    } else {
        Header(("Editing: " + std::string(curveName)).c_str(), boldFont);
    }
    ImGui::Text("Name: ");
    ImGui::SameLine();
    if(editingMode == CREATE_NEW){
        ImGui::InputText("##NAME", curveName, 30);
    } else {
        ImGui::Text("%s", curveName);
    }
    ImGui::Text("Start Time: ");
    ImGui::SameLine();
    ImGui::InputFloat("##start time of curve", &startTime);
    for(size_t i=0; i<movers.size(); i++) {
        auto mover = movers[i];
        if(ImGui::Button(("Delete##" + std::string(curveName) + std::to_string(i)).c_str())) {
            movers.erase(movers.begin()+i);
            i--;
            continue;
        }
        ImGui::Text("Duration: ");
        ImGui::SameLine();
        ImGui::InputFloat(("##duration of mover" + std::to_string(i)).c_str(), &mover->duration);
        if(ImGui::RadioButton(("Constant##" + std::string(curveName) + std::to_string(i)).c_str(), mover->moverType == CONSTANT)){
            mover->moverType = CONSTANT;
        }
        ImGui::SameLine();
        if(ImGui::RadioButton(("Bezier##" + std::string(curveName) + std::to_string(i)).c_str(), mover->moverType == BEZIER)){
            mover->moverType = BEZIER;
        }
        std::vector<float *> *points = &mover->points;
        for(size_t j=0; j<points->size(); j++) {
            float *point = (*points)[j];
            if(ImGui::Button(("X##Delete" + std::string(curveName) + std::to_string(i) + "Point" + std::to_string(j)).c_str())) {
                delete[] point;
                points->erase(points->begin()+j);
                j--;
                continue;
            }
            ImGui::SameLine();
            ImGui::InputFloat3(("##Input" + std::string(curveName) + std::to_string(i) + "Point" + std::to_string(j)).c_str(), point);
        }
        if(ImGui::Button(("Add Point##" + std::to_string(i)).c_str()))
            mover->points.push_back(new float[3]{0,0,0});
    }
    if(ImGui::Button("Add Mover")){
        movers.push_back(std::make_shared<ObjectMoverForGui>());
    }
    if(ButtonCenteredOnLine("Save")){
        bool saveSucceed = true;
        std::vector<std::shared_ptr<ObjectMover>> objectsForSave;
        auto nextStartTime = startTime;
        for(auto m : movers) {
            if(m->points.size() == 0){
                project->guiStates.push(std::make_shared<ErrorMsgState>("Movers must have 1 points at least"));
                saveSucceed = false;
                break;
            }
            if(m->moverType == CONSTANT && m->points.size() > 1) {
                project->guiStates.push(std::make_shared<ErrorMsgState>("Constant movers may have only one point"));
                saveSucceed = false;
                break;
            }

            std::shared_ptr<ObjectMover> savedMover;
            if(m->moverType == CONSTANT) {
                savedMover = std::make_shared<ObjectMoverConstant>(Eigen::Vector3f(m->points[0][0],
                                                                                   m->points[0][1],
                                                                                   m->points[0][2]),
                                                                                   nextStartTime,
                                                                                   m->duration);
            } else {
                std::vector<Eigen::Vector3f> bezPoints;
                for(auto p : m->points) {
                    bezPoints.push_back(Eigen::Vector3f(p[0], p[1], p[2]));
                }
                savedMover = std::make_shared<ObjectMoverBezier>(bezPoints, nextStartTime, m->duration);
            }
            objectsForSave.push_back(savedMover);
            nextStartTime += m->duration;

        }
        if(saveSucceed) {
            if(editingMode == CREATE_NEW) {
                if (!project->AddMovementCurve(std::make_shared<ObjectMoverSplit>(objectsForSave, curveName)))
                    project->guiStates.push(std::make_shared<ErrorMsgState>("Curve: " +
                        std::string(curveName) + " exists"));
            } else {
                project->SetNewMoversForCurve(std::string(curveName), objectsForSave);
            }
            project->guiStates.pop();
        }

    }
    ImGui::SameLine();
    if(ButtonCenteredOnLine("Close")) {
        project->guiStates.pop();
    }

}

MediaSliderState::MediaSliderState(): GuiState(MEDIA_SLIDER){

}

void
MediaSliderState::Run(Project *project, std::vector<igl::opengl::Camera *> &camera, Eigen::Vector4i &viewWindow,
                      std::vector<DrawInfo *> drawInfos, ImFont *font, ImFont *boldFont) {
    BeginSliderWindow("Slider");
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + io.DisplaySize.x * 0.352);
    switch(project->getAnimationStatus()) {
        case PLAYING:
            if(ImGui::Button("Pause")){
                project->guiStates.pop();
                project->Pause();
            }
            ImGui::SameLine();
            if(ImGui::Button("Stop")){
                // restore version of previous state
                project->guiStates.pop();
                project->Stop();
            }
            ImGui::SameLine();
            if(ImGui::Button("Replay")){
                project->Replay();
            }
            break;
        case PAUSED:
        case STOPPED:
            project->guiStates.pop();
            break;
    }
    float time = (float)project->GetGlobalTime();
    float maxTime = project->maxTime();
    ImGui::SetNextItemWidth(io.DisplaySize.x * 0.8);
    if (ImGui::SliderFloat("##Time Scale", &time, 0, maxTime, "%.1f"))
    {
        project->SetGlobalTime((long)time);
    }
}
