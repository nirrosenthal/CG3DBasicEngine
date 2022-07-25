get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})
#configure_file("${PROJECT_NAME}/configuration.txt" "${PROJECT_NAME}/../configuration.txt")
#file(GLOB SANDBOX ${PROJECT_NAME}/*.cpp ${PROJECT_NAME}/*.h)
#add_executable(${PROJECT_NAME} ${SANDBOX})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}) # set the binary directory to build folder
set (VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}) # set the binary directory to build folder
find_package(Boost COMPONENTS filesystem system)
include(ExternalProject)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(${PROJECT_NAME}_bin Project.cpp Project.h main.cpp InputManager.h SceneShape.cpp SceneShape.h ObjectMover.cpp ObjectMover.h Gui.cpp Gui.h ProjectViewerData.cpp ProjectViewerData.h LayerManager.cpp LayerManager.h GuiState.cpp GuiState.h StyleManager.h SceneShader.cpp SceneShader.h ObjectMoverForGui.cpp ObjectMoverForGui.h imgui.cpp SceneCamera.h SceneCamera.cpp BoundingRectangle.cpp BoundingRectangle.h)
IF(Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME}_bin igl::core igl::opengl igl::opengl_glfw igl::opengl_glfw_imgui igl::png nlohmann_json::nlohmann_json Boost::filesystem Boost::system)
ELSE()
    target_link_libraries(${PROJECT_NAME}_bin igl::core igl::opengl igl::opengl_glfw igl::opengl_glfw_imgui igl::png nlohmann_json::nlohmann_json)
ENDIF()
