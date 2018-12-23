# Palmgine 3D
PalmGine-3D is a game engine that has been done for the [Game Engines](https://www.upc.edu/estudispdf/guia_docent.php?codi=804246&lang=ing) subject in the 3rd year of the [Game Design and Development](https://www.citm.upc.edu/ing/estudis/graus-videojocs/) grade of [CITM](https://www.citm.upc.edu/ing/) ([UPC](https://www.upc.edu/en?set_language=en)). It has been done in a four month period and tries to be a basic Unity like engine with one high level system.

The high level system we’ve chosen is the implementation of an audio engine called [Wwise]( https://www.audiokinetic.com/products/wwise/), so in this engine you are able to create audio listeners, audio sources and audio reverb zones to give audio to the games made with it.

## Team Members
!["TeamPhoto"](TemPhoto.png)

### [Francisco Ruiz Giménez](https://github.com/botttos)

(guy in the left)

### [Manel Mourelo Montero](https://github.com/manelmourelo)

(guy in the right)

## Main Core Sub-Systems

### Module Scene
* Manage all the in the scene and the components attached to them.
* It allows you to save and load scenes (in binary format)

### File System
* Manage the folders used during the execution of the engine.

### Importer
* It allows you to import models and images and convert them into our own format.

### Time Manager
* It allows you to start, pause and stop the game-mode and controlls the deltatime of the objects in the scene.

### Resources Manager
* It avoid that you import repeated files.
* It has an “Asset” window that shows all user assets.

## Audio Sub-System

## Demo Video

## Links
[Code Repository](https://github.com/botttos/PalmGine-3D)

[Latest Release](https://github.com/botttos/PalmGine-3D/releases)

## License
### Palmgine 3D is under the [Apache License 2.0](https://opensource.org/licenses/Apache-2.0)

#### Copyright (c) 2018 [Fran Ruiz Giménez](https://github.com/botttos) and [Manel Mourelo Montero](https://github.com/manelmourelo)

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
