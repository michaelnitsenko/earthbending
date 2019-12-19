# Setup and usage

## Folders

### Earthbending

The folder contains Unreal Engine 4.23 project set up with third person camera view ready to be used with an earthbending algorithm that is under development in "sandbox" environment.

### sandbox

There are two environments setted up for experiments - "OpenGL" ("cpp") and "three.js" ("js" folder). First one contains only the scene with no elements in it, because the second one was chosen to be the actual sandbox for experimentations because of simplisity of writing js code.

## Usage

### ue4

UE4 project can be run and played as third-person game with WASD movement, mouse rotation and SPACE jump. Character is animated, reacts to collisions and physical based system is set up (can be experienced through gravity).

### sandbox

Sandbox environment can be used by opening "index.html" in "js" folder. There is basic "three.js" scene set up with two meshes - terrain and cube (character). Terrain uses depth material to nicely represent changes in mesh structure under earthbending modifications. Cube uses normal material to be clearly visible and easily distinguishable from the environment.

Cube can be moved by WASD keys, where W - forward, S - backward, A - left rotation, D - right rotation. A third person camera was implemented to track cube movement. Earthbending can be performed using up and down arrows and depends on cube position. Terrain mesh is built from a heightmap that is represented as two dimentional grid of heights and earthbending changes heights of vertices in action radius that is in from of the cube.
