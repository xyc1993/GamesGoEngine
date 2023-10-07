#pragma once

#include <vector>

namespace GamesGoEngine
{
    class TextureLoader
    {
    public:
        static unsigned int LoadTexture(const char* path, bool transparencyEnabled = false, bool sRGB = true);
        static unsigned int LoadCubemap(std::vector<const char*> faces, bool transparencyEnabled = false, bool sRGB = true);
        static void UnloadTexture(unsigned int texture);
    };
}