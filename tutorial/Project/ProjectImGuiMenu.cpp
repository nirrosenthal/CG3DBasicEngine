//
// Created by ditto on 29/06/2022.
//

#include "ProjectImGuiMenu.h"

#include "MenuManager.h"
#include "Project.h"

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
    ImGui::Begin(
            "Viewer", p_open,
            window_flags
    );

    ImGui::SetWindowPos(ImVec2((float)0, (float)0), ImGuiCond_Always);
    ImGui::SetWindowSize(ImVec2((float)0, (float)0), ImGuiCond_Always);
    ImGui::End();
    no_move = true;
    no_resize = true;
    ImGui::Begin(
            "Viewer", p_open,
            window_flags
    );

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

    if (ImGui::CollapsingHeader("Layers", ImGuiTreeNodeFlags_DefaultOpen)) {
        for (size_t i = 0; i < ((ProjectViewerData *)viewer->data())->layers.size(); i++)
        {
            if(ImGui::Checkbox((((ProjectViewerData *)viewer->data())->layers.at(i).name).c_str(),
                               &(((ProjectViewerData *)viewer->data())->layers.at(i)).isShown)){
                std::cout<<"layer changed:"<<(((ProjectViewerData *)viewer->data())->layers.at(i)).isShown<<std::endl;
                MenuManager::OnLayerChange((((ProjectViewerData *)viewer->data())->layers.at(i).name),(((ProjectViewerData *)viewer->data())->layers.at(i).isShown));
            }
        }
        ImGui::InputText("",((ProjectViewerData *)viewer->data())->layer_name);
        if(ImGui::Button("Add Layer3")){
            MenuManager::OnAddLayer(((ProjectViewerData *)viewer->data())->layer_name,true);
            ((ProjectViewerData *)viewer->data())->layers.emplace_back(((ProjectViewerData *)viewer->data())->layer_name, true);
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
        if (ImGui::Checkbox("Play Animation", &(viewer->isActive)))
        {
            if (viewer->isActive)
                viewer->Activate();
            else
                viewer->Deactivate();
            ((Project*)viewer)->Play();
            viewer->Animate();
        }

        // Adding to viewer time param
        if (ImGui::CollapsingHeader("Animation time", ImGuiTreeNodeFlags_DefaultOpen))
        {
            Project* project = (Project*)viewer;
            ImGui::DragFloat("Max time ", &(project->max_time), 0.05f, 0.0f, 100.0f, "%.0f");
            if (ImGui::SliderFloat("Time Scale", &(project->time), 0, (project)->max_time , "%.1f"))
            {
                std::cout << project->time << std::endl;
            }
        }
    }


    // Draw options
//   if (ImGui::CollapsingHeader("Draw Options", ImGuiTreeNodeFlags_DefaultOpen))
//   {
//     if (ImGui::Checkbox("Face-based", &(viewer->data()->face_based)))
//     {
//       viewer->data()->dirty = MeshGL::DIRTY_ALL;
//     }

// //
// //    make_checkbox("Show texture", viewer->data().show_texture);
// //    if (ImGui::Checkbox("Invert normals", &(viewer->data().invert_normals)))
// //    {
// //      viewer->data().dirty |= igl::opengl::MeshGL::DIRTY_NORMAL;
// //    }
//     make_checkbox("Show overlay", viewer->data()->show_overlay);
//     make_checkbox("Show overlay depth", viewer->data()->show_overlay_depth);

//     ImGui::ColorEdit4("Line color", viewer->data()->line_color.data(),
//         ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel);
//     ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);
//     ImGui::DragFloat("Shininess", &(viewer->data()->shininess), 0.05f, 0.0f, 100.0f);
//     ImGui::PopItemWidth();
//   }

//   // Overlays
//   if (ImGui::CollapsingHeader("Overlays", ImGuiTreeNodeFlags_DefaultOpen))
//   {
//     make_checkbox("Wireframe", viewer->data()->show_lines);
//     make_checkbox("Fill", viewer->data()->show_faces);

//   }
    ImGui::End();
}
