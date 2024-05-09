# Dynamic Wall Creation System in Unreal Engine

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

- Adds spline points at runtime using the left click of the mouse and for creating a new structure use right-click.
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
- Allows users to undo the last spline point added.
- Toggles between different creation modes, enabling or disabling the ability to create walls and new splines.

## Usage

- Left-click to add spline points and draw walls.
- Right-click to start a new wall segment.
- Press 'Z' to undo the last spline point added.
- Press 'C' to toggle between wall creation mode and new spline creation mode.

## Installation

1. Clone the repository to your local machine.
2. Open the Unreal Engine project in the Unreal Editor.
3. Compile the project to ensure all C++ classes are compiled.
4. Play the game in the editor or package the project for distribution.

## Dependencies

- Unreal Engine 5.2
