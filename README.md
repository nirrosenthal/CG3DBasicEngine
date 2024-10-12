# EngineForAnimationCourse
#### _Submitted by Nadav Haik Baruch, Matan Cohen and Nir Rosenthal_


Graphics and animation Engine based on Libigl, with loads of groundbreaking features and crazy shaders!

# Shaders
### Basic Shaders:
1) blackShader - paints the shape in black
2) invisbleShader - makes a shape completely invinsible 

### Special Shaders:
1) basicShader - draws all keyboard characters in red on a shape
2) cubemapShader - draws skyline on a shape
3) distorted - time dynamic psycho blue/green drawings
4) RGB_oneColor - paints shape with given vec4float RGB values
5) mud - mouse dynamic mud drawings
6) palette_oneColor - paints shape with color palette chosen value


### Special Shaders specifically for the background:
1) newtonMoving - draws a time dynamic Newton's fractal on big shapes
2) overlay - draws color pallette on big shapes
3) snow - paints snow on big shapes


# Features

## Menu Features
#### Pop-up mechanics
- The GUI is based on pop-up windows. When a window closes, the previous window automatically opens!
#### Theme 
- Choose between light and dark mode for the menu
#### Background Shader 
- Change the shader for the background of the scene
#### Shapes : 
###### Create a new shape:
- Name - choose a name for the new shape
- Source:
  - Choose to load a basic shape type or load a shape type from file!
  - Simplicity - the gui automatically filters all the ".obj" files. No special dialog box is needed in order to locate them.
- Size - choose the size of the shape
- Shader - choose a shader for the shape from a menu of shaders!
- Layer - choose a layer for the new shape!
- Movement Curves - choose a Bézier curve for the shape from the movement curves you defined!
- The checkbox automatically picks the shape and enables moving it with the keyboard.
###### Existing shape edit - Click on a shape name to edit the shape:
- Size - change the size of the shape
- Shader - change the shader of the shape
- Layer - change the layer of the shape
- Movement curve - change the movement curve of the shape
 #### Search for Shaders and Edit Selected Shader
- Search a shader to show it under Edit Selected Shader! Edit the shader's parameters, or control it's transparency! Each shader requires different parameters, so we recommend to try all of them!
- #### Shaders :
- Adding a json file to the shader makes it editable, and allows the user to edit its uniforms.
- The json also supports global parameters - like mouse position, global time, etc.
 #### Layers
- Show/hide layers from the list
- Create a new layer
- Delete a layer (if it has no shapes).
#### Movement Curves
###### add a movement curve:
- Add a name to the curve
- Add a start time for the curve, in what time should the curve begin movement
- Add movers - with a specific form
#### Cameras
- Camera split option - choose the screen split mode for the design mode IN RUNTIME! choose between vertical split (split X) and horizontal split (split Y)
- Select camera for screen 1 - choose the camera you want to see on the left screen (if split X is active) or on the top screen (if split Y is active)
- Select camera for screen 2 - choose the camera you want to see on the right screen (if split X is active) or on the bottom screen (if split Y is active)
- Select camera for screen animation - choose the camera you want to see when in animation mode
- Add Camera: choose the name, seek the position (x,y,z) and movement curve for the new camera
#### Animation
- Play the animation: cancels the screen splitting and changes the camera to be the camera set for the animation mode. choose to pause/stop/replay the animation!

## Keyboard Features
#### Mouse controls
- Click and drag with the left mouse button to move the camera around
- Click and drag with the right mouse button to pick a zone in the screen which you can zoom in to, or move the shapes in it by clicking and dragging with the left mouse button
#### Keyboard keys
- ESC closes the current pop-up window.
- Click CTRL in split screen mode to switch the camera control from screen 1 to screen 2
- Use the arrows to move the camera up/down/left/right when no object is selected.
- When objects are selected, use the arrows and the +/- buttons to move it.

### Run Instruction:
1. Clone or download the project from the github link
2. Open the project folder in CLion
3. Open the Open Project Wizard, choose CMake and enter those params:
    - Name: Debug
    - Build type: Debug
    - Toolchain: Use Default (Visual Studio)
    - Generator: Use Default (Ninja)
    - Build Directory: cmake-build-debug
    - Build options: -j 6
4. Choose the "Project_bin | Debug" configuration (open the menu next to the Run button)
5. Press Run (Shift + F10)







