# GamesGoEngine
GamesGo Engine is my personal game engine project. I've decided to create a general purpose game engine in order to learn more about game engine development and how the game engines work in detail. I was always interested how things work behind the scenes so after working with both Unity and Unreal I've decided to tackle game engine development myself.

After opening "GamesGoEngine.sln" with Visual Studio just compile. Project is set up in a way that should work on Windows with Visual Studio 2022.
There are hardcoded asset references so if you want to run the build from built .exe file, you need to copy "res" folder from "GamesGoEngine" subfolder to the folder where .exe is located. "assimp-vc142-mtd.dll" needs to be copied as well. "imgui.ini" is not needed as it will be generated on application start but it contains predefined UI settings (like panels' positions) so it's recommended to copy it as well to have an organized UI.

Devlog and current status of the engine: https://macgry.wordpress.com/
