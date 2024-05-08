# Unreal Engine Assignment - Week 3

## Objective
This project aims to implement a dynamic wall creation system in Unreal Engine using C++. Players can interact with the environment to create walls dynamically, and the camera zoom can be adjusted using the mouse wheel.

## Classes and Functionality

### ATopDownPawn
#### Description
- A pawn class for a top-down perspective.
- Includes a camera component for the top-down view.
- Excludes a visible player mesh.

#### Functionality
- Basic movement controls using WASD keys for lateral movement on the XY plane.
- Mouse wheel controls for zooming in and out.
- Adjusts camera angle and zoom level dynamically.

### AWallSpline
#### Description
- An actor class responsible for creating spline points and walls dynamically.

#### Functionality
- Adds spline points at runtime Using the left click of the mouse and for creating new structure Use Right Click.
- Generates continuous wall meshes along the spline.
- Straight geometrical blocks are created without curves.

### AWallBuilderController
#### Description
- A custom player controller class.
- Reacts to mouse input to add spline points and draw walls dynamically.

#### Functionality
- Adds spline points and draws walls based on mouse input.
- Starts a new wall segment after the previous one is completed.
- Handles notifications for the beginning and end of wall drawing.

## Usage
1. Clone the repository to your local machine.
2. Open the Unreal Engine project in the Unreal Editor.
3. Compile the project to ensure all C++ classes are compiled.
4. Play the game in the editor or package the project for distribution.