# Palmgine 3D is a project consistent of creating a Game Engine.

To see the code of this assignment check branch called "Assignment3"
Link: https://github.com/botttos/PalmGine-3D/tree/Assignment3

Hope you like it. 

# QUICK START #

1) To play with the audio scene demo go to File -> Load -> AudioScene
2) You can find: 
	- Listener GameObject with a listener and audiosource wich blends between two musics in loop. Music tracks are 2D positioned.
	- Train GameObject that is moving from point A to point B with a 3D spatial audio.
	- Tunnel GameObject with audio distort. This zone can be a sphere or a cube. Size is modificable.
3) Audio settings. You can:
	- Introduce the name of the AudioClip.
	- Time to swap between musics
	- Play/stop
	- Priority
	- Volume
	- Pitch
	- Only 2D tracks: change pan.
4) List of AudioClips loaded in the soundbank:

* MUSICS
2D	"music_TheDillards_Dooley"
2D	"music_TheGrowlers_GoingGetsTuff"
2D	"background_tracks" -> Event that changes between two songs when the game state is changed.
* FX
3D	"train"

# CAMERA CONTROLS (Like Unity)

- Mouse left + Alt-left: Move Camera arround model pivot.
- Mouse right click: Move Camera with static position.
- Mouse right click + WASD keys: Navigate arround the scene.
- Mouse wheel: Move Camera forward and backward.
- Mouse middle: Panning.
- F: Center the Camera on an object.
- Shift to speed up the camera's velocity.

# FEATURES
- WWISE 2017 INTEGRATION
- Listener, audio source, zones to distorsionate audio, component movement.
- Configuration options (Application, window, scene, render, hardware, volume, camera, light and input).
- Inspector shows and applies transform propierties, mesh information and texture information.
- Console with LOG control.
- Game object hierarchy that applies all transformations correctly 
- Own file format.
- Quadtree that accelerates an internal list of static Gameobjects.
- Camera (frustum culling, near/far plane, modifiable FOV and aspect ratio).
- Guizmos can transform., rotate and scale.
- Assets folder.
- GameMode with modifiable time scale. Start/Pause/Stop.
- MousePicking (Mouse left click).
- Binary scene serialization (Allow to save and load scenes).
- Application/Scene window can be resized and the rendering window adapts to the new aspect ratio.
- All meshes use a bounding volume (AABB). Can be discarded with frustum culling.
- All resources use reference counting (One texture/mesh is only once in memory regardless of how many gameobject use it).
- Resource management:
   - “Asset” window that shows all user assets.
   - Upon start, all resources not managed inside Assets are generated in Library.

# VERSION CONTROL
## v.1.5
- WWISE 2017 INTEGRATION
- Listener, audio source, zones to distort audio, component movement.

# VERSION CONTROL
## v.1.0
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
- Game Object hierarchy implemented with components.
- Transformations can be applied to game objects
- When a new item is dropped, the system will generate a binary file (engine own format) and read the item from here.
- Textures are written and readed in DDS format.
- Quadtree optimization.
- Camera uses frustrum culling, far/near plane, FOV and aspect ratio.
- Guizmos implemented.
### // * NEW * //
- GameMode with own timer scale that can modify time speed.
- Assets folder shown in the engine.
- Mouse Picking to pick objects in the scene.
- Scene serialization using binary format.
- Load scenes.
- Load textures and apply them to the selected object.

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
