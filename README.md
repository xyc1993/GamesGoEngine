# GamesGoEngine
GamesGo Engine is my personal game engine project. I've decided to create a general purpose game engine in order to learn more about game engine development and how the game engines work in detail. I was always interested how things work behind the scenes so after working with both Unity and Unreal I've decided to tackle game engine development myself.

Currently I'm focusing on rendering via OpenGL. However there is simple UI with options to change graphic settings or change position, rotation and scale of game objects in the scene. Selected game objects have an outline and 3 lines indicating their up, right, forward directions are shown. There's also an option to log messages in the panel dedicated to logs.
So far both forward and deferred rendering is implemented. Materials used by mesh renderers can accept vertex, fragment and geometry shaders.

HDR tonemapping, gamma correction and bloom are effects available in the UI.
![image](https://github.com/xyc1993/GamesGoEngine/assets/20773439/ef8de0a6-bdbd-42ed-a5c4-5892986a9997)

Shadow mapping is implemented. It supports directional lights, point lights and spot lights. Several lights of each type can be handled.
![image](https://github.com/xyc1993/GamesGoEngine/assets/20773439/116dd584-f655-4640-aa1f-0c80e7e7ddc6)
