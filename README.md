# Palmgine 3D is a project consistent of creating a Game Engine.

To work with PalmGine 3D you must drag and drop the files on the window.

To see the code of this assignment check branch called "Assignment2"
Link: https://github.com/botttos/PalmGine-3D/tree/Assignment2

Hope you like it.  

# INNOVATION

- Scene serialization in binary to make it faster.

# CAMERA CONTROLS (Like Unity)

- Mouse right click: Move Camera with static position
- Mouse right click + WASD keys: Navigate arround the scene
- Mouse left + Alt-left: Move Camera arround model pivot
- Mouse wheel: Move Camera forward and backward
- Mouse middle: Panning
- F: Center the Camera on an object

# FEATURES

- Configuration options (Application, window, scene, render, hardware, volume, camera, light and input).
- Inspector shows and applies transform propierties, mesh information and texture information.
- Console with LOG control.
- Game object hierarchy.
- Own file format.
- Quadtree.
- Camera (frustum culling, near/far plane, FOV and aspect ratio).
- Guizmos.
- Assets folder.
- GameMode with modifiable time scale.
- MousePicking.
- Binary scene serialization (Allow to save and load scenes).

# VERSION CONTROL
## v.0.7
- Load and draw one .fbx or .obj file with one channel diffuse.
- Configuration options (Application, window, scene, render, hardware, volume, camera, light and input).
- Inspector shows transform propierties, mesh information and texture information.
- Console with LOG control.
- Drag and drop.
- Unity-like camera.
- Wireframe mode
- Texture alpha tolerance
- Light control
- Window settings are saved in parson
### // * NEW * //
- Game Object hierarchy implemented with components.
- Transformations can be applied to game objects
- When a new item is dropped, the system will generate a binary file (engine own format) and read the item from here.
- Textures are written and readed in DDS format.
- Quadtree optimization.
- Camera uses frustrum culling, far/near plane, FOV and aspect ratio.
- Guizmos implemented.

# VERSION CONTROL
## v.0.5.2
- Load and draw one .fbx or .obj file with one channel diffuse.
- Configuration options (Application, window, scene, render, hardware, volume, camera, light and input).
- Inspector shows transform propierties, mesh information and texture information.
- Console with LOG control.
- Drag and drop.
- Unity-like camera.
- Wireframe mode
- Texture alpha tolerance
- Light control
- Window settings are saved in parson
- Code polished

## v.0.5.1
- Load and draw one .fbx or .obj file with one channel diffuse.
- Configuration options (Application, window, scene, render, hardware, volume, camera, light and input).
- Inspector shows transform propierties, mesh information and texture information.
- Console with LOG control.
- Drag and drop.
- Unity-like camera.
- Wireframe mode
- Texture alpha tolerance
- Light control

## v.0.5
- Load and draw one .fbx or .obj file with one channel diffuse.
- Configuration options (Application, Window, Scene, Hardware, Volume, Camera and Input).
- Inspector shows transform propierties, mesh information and texture information.
- Console with LOG control.
- Drag and drop.
- Unity-like camera.

---

### Palmgine 3D is under the Apache License 2.0

#### Copyright (c) 2018 Fran Ruiz Giménez and Manel Mourelo Montero

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
