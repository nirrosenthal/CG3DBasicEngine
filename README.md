# EngineForAnimationCourse
#### _Submitted by Nadav Haik Baruch, Matan Cohen and Nir Rosenthal_


Graphics and animation Engine based on Libigl, with loads of groundbreaking features and crazy shaders!



# Features

## Menu Features
#### Theme 
- Choose between light and dark mode for the menu
#### Background Shader 
- Change the shader for the background of the scene
#### Viewing Options
- Center Object - $ $ $ $ $ $ $ $ $ $ $ 
- Fov slider - $ $ $ $ $ $ $ $ $ $ $ 
- Orthographic view - $ $ $ $ $ $ $ $ $ $ $ 
#### Shapes : 
###### Create a new shape:
- Name - choose a name for the new shape
- Source - choose to load a basic shape type or load a shape type from file!
- Size - choose the size of the shape
- Shader - choose a shader for the shape from a menu of shaders!
- Layer - choose a layer for the new shape!
- Movement Curves - choose a bezier curve for the shape from the movement curves you defined!
###### Existing shape edit - Click on a shape name to edit the shape:
- Size - change the size of the shape
- Shader - change the shader of the shape
- Layer - change the layer of the shape
- Movement curve - change the movement curve of the shape
 #### Search for Shaders and Edit Selected Shader
- Search a shader to show it under Edit Selected Shader! Edit the shader's parameters, or control it's transparency! Each shader requires different parameters, so we recommend to try all of them! 
 #### Layers
- Show/hide layers from the list
- Create a new layer
- Delete a layer (layer list should not be less than 1!)
#### Movement Curves
###### add a movement curve:
- Add a name to the curve
- Add a start time for the curve, in what time should the curve begin movement
- Add movers: $ $ $ $ $ $ $ $ $ $ $ 
#### Cameras
- Camera split option - choose the screen split mode for the design mode IN RUNTIME! choose between vertical split (split X) and horizontal split (split Y)
- Select camera for screen 1 - choose the camera you want to see on the left screen (if split X is active) or on the top screen (if split Y is active)
- Select camera for screen 2 - choose the camera you want to see on the right screen (if split X is active) or on the bottom screen (if split Y is active)
- Add Camera: choose the name, angle, width/height ration, near, far and a movement curve for a new camera you want to add
#### Animation
- Play the animation: cancels the screen splitting and changes the camera to be the camera set for the animation mode. choose to pause/stop/replay the animation!

## Keyboard Features
#### Mouse controls
- Click and drag with the left mouse button to move the camera around
- Click and drag with the right mouse button to pick a zone in the screen which you can zoom in to, or move the shapes in it by clicking and dragging with the left mouse button
#### Keyboard keys
- Double click ESC to exit the window
- Click CTRL in split screen mode to switch the camera control from screen 1 to screen 2
- Use the arrows to move the camera up/down/left/right
# GitHub links:
Our engine - https://github.com/nadavhaik/CG3DBasicEngine

### Run Instruction:
1. Clone or download the project from the github link
2. Download Cmake from the link in the site
3. Run Cmake, choose the project folder with "Browse Source".
4. Copy the project folder path, and in "Where to build the binaries:", paste the path and add "/build" at the end
5. Click configure -> yes -> finish
6. When the configuration is done, click configure again
7. When the second configuration is done, click Generate
8. When the generation is done click Open Project
9. Right click Project_bin, click "Set as Startup Project"
10. Right click Project_bin, click "Properties", and under "Configuration Properties -> Debugging" change "Working Directory" from "$(ProjectDir)" to "$(SolutionDir)" 
11. Press start in Visual Studio (Ctrl + F5)










