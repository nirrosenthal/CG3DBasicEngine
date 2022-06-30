//
// Created by ditto on 29/06/2022.
//

#include "ProjectImGuiMenu.h"
#include "MenuManager.h"
#include "Project.h"
#include "stb_image.h"

std::map<std::string, Theme> THEMES = {
        {"Dark Mode", DARK},
        {"Light Mode", LIGHT}
};

std::map<Theme, std::string> THEMES_REV = {
        { DARK, "Dark Mode"},
        {LIGHT,"Light Mode"}
};
std::string DEFAULT_THEME = "Dark Mode";

Icon::~Icon() {
    delete[] texture;
}

void setDarkMode() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;
    style.ScrollbarRounding = 0;

    style.Colors[ImGuiCol_Text]                  = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);

    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
    style.Colors[ImGuiCol_Border]                = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_Header]                = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);

    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    style.Colors[ImGuiCol_PlotLines]             = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
}

void setLightMode() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(0.31f, 0.25f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.74f, 0.74f, 0.94f, 1.00f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.62f, 0.70f, 0.72f, 0.56f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.95f, 0.33f, 0.14f, 0.47f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.97f, 0.31f, 0.13f, 0.81f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.42f, 0.75f, 1.00f, 0.53f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.65f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.40f, 0.62f, 0.80f, 0.15f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.39f, 0.64f, 0.80f, 0.30f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.28f, 0.67f, 0.80f, 0.59f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 0.48f, 0.53f, 0.67f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.48f, 0.47f, 0.47f, 0.71f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.31f, 0.47f, 0.99f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(1.00f, 0.79f, 0.18f, 0.78f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.42f, 0.82f, 1.00f, 0.81f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.72f, 1.00f, 1.00f, 0.86f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.65f, 0.78f, 0.84f, 0.80f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.75f, 0.88f, 0.94f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.55f, 0.68f, 0.74f, 0.80f);//ImVec4(0.46f, 0.84f, 0.90f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.60f, 0.60f, 0.80f, 0.30f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.99f, 0.54f, 0.43f);
    style.Alpha = 1.0f;
    style.FrameRounding = 4;
    style.IndentSpacing = 12.0f;
}

Icon::Icon(const char* filename) {
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        throw std::runtime_error("Could not load: " + std::string(filename));

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *texture = image_texture;
    width = image_width;
    height = image_height;


}

GLuint *Icon::getTexture() {
    return texture;
}

int Icon::getWidth() {
    return width;
}

int Icon::getHeight() {
    return height;
}


ProjectImGuiMenu::ProjectImGuiMenu() :
 igl::opengl::glfw::imgui::ImGuiMenu(),
 theme(THEMES[DEFAULT_THEME])
// playButton("textures/play_button.jpg")
 {

 }

void ProjectImGuiMenu::init(Display *disp) {

    ImGuiMenu::init(disp);
    ImGuiIO& io = ImGui::GetIO();
    font = io.Fonts->AddFontFromFileTTF("textures/Calibri.ttf", 12);
}

IGL_INLINE void ProjectImGuiMenu::draw_viewer_menu(igl::opengl::glfw::Viewer *viewer, std::vector<igl::opengl::Camera*> &camera,Eigen::Vector4i& viewWindow,std::vector<DrawInfo *> drawInfos)
{
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
    switch (theme) {
        case DARK:
            setDarkMode();
            break;
        case LIGHT:
            setLightMode();
            break;
    }
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
            int savedIndx = viewer->selected_data_index;
            viewer->open_dialog_load_mesh();
            if (viewer->data_list.size() > viewer->parents.size())
            {
                viewer->parents.push_back(-1);
                ((ProjectViewerData *)viewer->data_list.back())->set_visible(false, 1);
                ((ProjectViewerData *)viewer->data_list.back())->set_visible(true, 2);
                viewer->data_list.back()->show_faces = 3;
                viewer->selected_data_index = savedIndx;
            }
        }
        ImGui::SameLine(0, p);
        if (ImGui::Button("Save##Mesh", ImVec2((w-p)/2.f, 0)))
        {
            viewer->open_dialog_save_mesh();
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
    ImGui::ColorEdit4("Background", drawInfos[1]->Clear_RGBA.data(),
                      ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel);
    auto layers = ((Project *)viewer)->layerManager.layers;
    if (ImGui::CollapsingHeader("Layers", ImGuiTreeNodeFlags_DefaultOpen)) {
        for (auto const &layerEntry: layers)
        {
            std::string layerName = layerEntry.first;
            auto layer = layerEntry.second;
            bool isShown = !layer->isHidden();
            if(ImGui::Checkbox((layerName).c_str(), &isShown)){
                layer->changeHidden(!isShown);
                std::cout<<"layerEntry changed:"<< layer->isHidden() <<std::endl;
            }
        }


        ImGui::InputText("##NEW LAYER NAME", newLayerName, 30);
        ImGui::SameLine();
        if(ImGui::Button("Add")){
            auto newName = std::string(newLayerName);
            if(!newName.empty()) {
                ((Project *) viewer)->layerManager.addLayer(newName);
                delete[] newLayerName;
                newLayerName = strdup("");
            }
        }
    }

    std::string tempStr = "material" + (std::to_string(((ProjectViewerData *)viewer->data())->material_indx));
    const char* current_material_item = tempStr.c_str();
    if (ImGui::BeginCombo("##materials combo", current_material_item))
    {
        for (size_t n = 0; n < viewer->materials.size(); n++)
        {
            bool is_selected = strcmp(current_material_item ,("material" + (std::to_string(n))).c_str())==0;
            std::cout<<"compare: "<<std::string(current_material_item)<<" with "<<("material" + (std::to_string(n))).c_str()<<std::endl;
            if (ImGui::Selectable(("material" + (std::to_string(n))).c_str(), is_selected))
            {
                current_material_item = ("material" + (std::to_string(n))).c_str();
                ((ProjectViewerData *)viewer->data())->material_indx = n;
                MenuManager::OnSelectMaterial(n);
            }
            if (is_selected){
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    if (ImGui::CollapsingHeader("Camera Split", ImGuiTreeNodeFlags_DefaultOpen)) {
        if(ImGui::RadioButton("No Split",&(((ProjectViewerData *)viewer->data())->camera_split),0)){
            MenuManager::OnCameraSplitChange(MenuManager::CameraSplitMode::no_split);
        }
        if(ImGui::RadioButton("Split x",&(((ProjectViewerData *)viewer->data())->camera_split),1)){
            MenuManager::OnCameraSplitChange(MenuManager::CameraSplitMode::split_x);
        }
        if(ImGui::RadioButton("Split y",&(((ProjectViewerData *)viewer->data())->camera_split),2)){
            MenuManager::OnCameraSplitChange(MenuManager::CameraSplitMode::split_y);
        }

        ImGui::Text("Select Camera for screen 1:");
        const char* current_item = ((ProjectViewerData *)viewer->data())->cameras.at(((ProjectViewerData *)viewer->data())->cameraScreen1Indx).c_str();
        if (ImGui::BeginCombo("##cameras combo 1", current_item))
        {
            for (size_t n = 0; n < ((ProjectViewerData *)viewer->data())->cameras.size(); n++)
            {
                bool is_selected = (current_item == ((ProjectViewerData *)viewer->data())->cameras.at(n).c_str());
                if (ImGui::Selectable(((ProjectViewerData *)viewer->data())->cameras.at(n).c_str(), is_selected))
                {
                    current_item = ((ProjectViewerData *)viewer->data())->cameras.at(n).c_str();
                    ((ProjectViewerData *)viewer->data())->cameraScreen1Indx = n;
                    std::cout<<"Selected in combo: "<<current_item<<std::endl;
                }
                if (is_selected){
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }
        ImGui::Text("Select Camera for screen 2:");
        const char* current_item2 = (((ProjectViewerData *)viewer->data())->cameras.at(((ProjectViewerData *)viewer->data())->cameraScreen2Indx)).c_str();

        if (ImGui::BeginCombo("##cameras combo 2", current_item2))
        {
            for (size_t n = 0; n < ((ProjectViewerData *)viewer->data())->cameras.size(); n++)
            {
                bool is_selected = (current_item2 == ((ProjectViewerData *)viewer->data())->cameras.at(n).c_str());
                if (ImGui::Selectable(((ProjectViewerData *)viewer->data())->cameras.at(n).c_str(), is_selected))
                {
                    current_item2 = ((ProjectViewerData *)viewer->data())->cameras.at(n).c_str();
                    ((ProjectViewerData *)viewer->data())->cameraScreen2Indx = n;
                    std::cout<<"Selected in combo: "<<current_item2<<std::endl;
                }
                if (is_selected){
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::Text("Camera name:");
        ImGui::InputText(".",(((ProjectViewerData *)viewer->data())->camera_name));
        if(ImGui::Button("Add Camera")){
            ((ProjectViewerData *)viewer->data())->cameras.push_back(((ProjectViewerData *)viewer->data())->camera_name);
            MenuManager::OnAddCamera(((ProjectViewerData *)viewer->data())->camera_name);
        }
    }

    if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
    {
//        ImGui::Image((void*)(intptr_t)(playButton.getTexture()),
//                     ImVec2(playButton.getWidth(), playButton.getHeight()));
        switch(((Project*)viewer)->getAnimationStatus()) {
            case PLAYING:
                if(ImGui::Button("Pause")){
                    ((Project*)viewer)->Pause();
                }
                ImGui::SameLine();
                if(ImGui::Button("Stop")){ //                 if(ImGui::Button("Stop", ImVec2(60, 30)){
                    ((Project*)viewer)->Stop();
                }
                ImGui::SameLine();
                if(ImGui::Button("Replay")){
                    ((Project*)viewer)->Replay();
                }
                break;
            case PAUSED:
//                if(ImGui::ImageButton((void*)(intptr_t)(playButton.getTexture()),
//                                      ImVec2(playButton.getWidth(), playButton.getHeight()))) {
//                    ((Project*)viewer)->Play();
//                }
                if(ImGui::Button("Play")){
                    ((Project*)viewer)->Play();
                }
                ImGui::SameLine();
                if(ImGui::Button("Stop")){
                    ((Project*)viewer)->Stop();
                }
                ImGui::SameLine();
                if(ImGui::Button("Replay")){
                    ((Project*)viewer)->Replay();
                }
                break;
            case STOPPED:
                if(ImGui::Button("Play")){
                    ((Project*)viewer)->Play();
                }
                break;
        }

        // Adding to viewer time param
        if (ImGui::CollapsingHeader("Slider", ImGuiTreeNodeFlags_DefaultOpen))
        {
            Project* project = (Project*)viewer;
            float time = (float)project->GetGlobalTime();
            float maxTime = project->maxTime();
            ImGui::DragFloat("End time", &maxTime, 0.05f, 0.0f, 100.0f, "%.0f");
            if (ImGui::SliderFloat("##Time Scale", &time, 0, maxTime, "%.1f"))
            {
                project->SetGlobalTime((long)time);
            }
        }
    }


    ((Project*)viewer)->menuSize = ImGui::GetWindowSize();

    ImGui::End();
    ImGui::PopFont();





//    ImGui::Begin(
//            "## SLIDER", p_open,
//            window_flags
//    );
//    switch (theme) {
//        case DARK:
//            setDarkMode();
//            break;
//        case LIGHT:
//            setLightMode();
//            break;
//    }
//    ImGui::SetWindowPos(ImVec2((float)300, 600), ImGuiCond_Always);
//    ImGui::SetWindowSize(ImVec2((float)400, (float)0), ImGuiCond_Always);
//
//    if(((Project*)viewer)->getAnimationStatus() != PLAYING) {
//        Project* project = (Project*)viewer;
//        ImGui::DragFloat("Max time ", &(project->max_time), 0.05f, 0.0f, 100.0f, "%.0f");
//        float time = (float)project->GetGlobalTime();
//
//        if (ImGui::SliderFloat("Time Scale", &time, 0, (project)->max_time , "%.1f"))
//        {
//            project->SetGlobalTime((long)time);
//        }
//
//    }


//    ImGui::End();
}

ProjectImGuiMenu::~ProjectImGuiMenu() {
    delete newLayerName;
    delete font;
}





