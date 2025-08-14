#pragma once

#include <raylib.h>
#include <string>
#include <map>

class TextureManager {
public:
    static Texture2D loadTexture(const std::string &path); // Load a texture from file
    static void unloadTexture(const std::string &path); // Unload a texture from memory
    static Texture2D getTexture(const std::string &path); // Get a texture by path, loading it if necessary

private:
    static std::map<std::string, Texture2D> textures; // Map to store loaded textures by path
};

