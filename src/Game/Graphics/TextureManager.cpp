#include "TextureManager.h"
#include <map>
#include <raylib.h>

 std::map<std::string, Texture2D> TextureManager::textures;

Texture2D TextureManager::loadTexture(const std::string& path) {
    if (textures.find(path) == textures.end()) {
        textures[path] = ::LoadTexture(path.c_str());
    }
    return textures[path];
}

void TextureManager::unloadTexture(const std::string& path) {
    if (textures.find(path) == textures.end()) {
        ::UnloadTexture(textures[path]);
        textures.erase(path);
    }
}

Texture2D TextureManager::getTexture(const std::string& path) {
    return textures[path];
}