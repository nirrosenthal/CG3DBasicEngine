//
// Created by ditto on 29/06/2022.
//

#include "Gui.h"
#include "Project.h"
#include "stb_image.h"



Icon::~Icon() {
    delete[] texture;
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
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

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


Gui::Gui() :
 igl::opengl::glfw::imgui::ImGuiMenu()
// playButton("textures/play_button.jpg")
 {

 }

void Gui::init(Display *disp) {
    ImGuiMenu::init(disp);
    ImGuiIO &io = ImGui::GetIO();
    font = io.Fonts->AddFontFromFileTTF("textures/Calibri.ttf", 25);
    boldFont = io.Fonts->AddFontFromFileTTF("textures/calibrib.ttf", 28);

}



IGL_INLINE void Gui::draw_viewer_menu(igl::opengl::glfw::Viewer *viewer,
                                      std::vector<igl::opengl::Camera*> &camera,
                                      Eigen::Vector4i& viewWindow,
                                      std::vector<DrawInfo *> drawInfos) {
    if(!((Project *) viewer)->IsGuiInitialized()) {
        ((Project *)viewer)->OpenNewWindow(std::make_shared<MenuState>(hidpi_scaling_, pixel_ratio_));
    }

    ((Project *) viewer)->GetCurrentWindow()->Run((Project *)viewer, camera, viewWindow, drawInfos, font, boldFont);


    ImVec2 topLeft = ImGui::GetWindowPos();
    ImVec2 winSize = ImGui::GetWindowSize();
    ImVec2 bottomRight(topLeft.x + winSize.x, topLeft.y + winSize.y);
    ((Project *)viewer)->UpdateWindowLocation(topLeft, bottomRight);

    ImGui::End();
}



Gui::~Gui() {
    delete font;
    delete boldFont;
}















