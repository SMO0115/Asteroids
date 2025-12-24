//
// Created by marcel on 10/10/25.
//

#include "AssetManager.h"

#include <cassert>
#include <filesystem>
#include <iostream>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#include <fstream>
#include "yyjson.h"

#include "Animation.h"
#include "Font.h"
#include "Sound.h"
#include "Texture.h"

#include "engine/modules/rendering/RenderModule.h"


namespace Engine::Assets {
bool AssetManager::loadTexture(Graphics::RenderSystem& renderer, const std::string& id, const std::string& path) {

    SDL_Surface* temp_surface = IMG_Load(path.c_str());

    if (temp_surface == nullptr) {
        std::cerr << "Failed to load image: " << path << " | " << IMG_GetError() << std::endl;
        return false;
    }

    SDL_Texture* sdl_texture = SDL_CreateTextureFromSurface(renderer.getSDLRenderer(), temp_surface);
    SDL_FreeSurface(temp_surface);

    if (sdl_texture == nullptr) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        return false;
    }


    auto engine_texture = std::make_unique<Texture>();

    engine_texture->texture = sdl_texture;
    engine_texture->width   = temp_surface->w;
    engine_texture->height  = temp_surface->h;

    m_textures[id] = std::move(engine_texture);

    return true;
}

bool AssetManager::loadAnimation(const std::string& id, const std::string& path) {

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open JSON file: " << path << std::endl;
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string fileContent = buffer.str();

    // Parse json
    yyjson_doc* doc = yyjson_read(&fileContent[0], fileContent.length(), 0);
    if (doc == nullptr) {
        std::cerr << "Error: Failed to parse JSON." << std::endl;
        return false;
    }

    yyjson_val* root   = yyjson_doc_get_root(doc);
    yyjson_val* frames = yyjson_obj_get(root, "frames");


    auto texture_it = m_textures.find(id);
    if (texture_it == m_textures.end()) {
        std::cerr << "Error: No texture found with ID: " << id << std::endl;
        // Handle the error, maybe return false
    }

    auto animation     = std::make_unique<Animation>();
    animation->texture = texture_it->second.get();
    animation->loop    = true;

    size_t      idx, max;
    yyjson_val* frame;
    const float DEFAULT_DURATION = 1.0f;  // Your default, e.g., 100ms
    yyjson_arr_foreach(frames, idx, max, frame) {

        std::string filename = yyjson_get_str(yyjson_obj_get(frame, "filename"));

        yyjson_val* frame_obj = yyjson_obj_get(frame, "frame");
        const int   x         = yyjson_get_int(yyjson_obj_get(frame_obj, "x"));
        const int   y         = yyjson_get_int(yyjson_obj_get(frame_obj, "y"));
        const int   w         = yyjson_get_int(yyjson_obj_get(frame_obj, "w"));
        const int   h         = yyjson_get_int(yyjson_obj_get(frame_obj, "h"));

        float duration;

        yyjson_val* duration_val = yyjson_obj_get(frame, "duration");

        if (duration_val != nullptr && yyjson_is_real(duration_val))
            duration = yyjson_get_real(duration_val);
        else
            duration = DEFAULT_DURATION;

        animation->frames.push_back({x, y, w, h});
        animation->durations.push_back(duration);
    }

    m_animations[id] = std::move(animation);

    yyjson_doc_free(doc);
    return true;
}


bool AssetManager::loadSound(const std::string& id, const std::string& path) {

    Mix_Chunk* shootSound = Mix_LoadWAV(path.c_str());
    Mix_VolumeChunk(shootSound, 5);
    if (shootSound == nullptr) {
        std::cerr << "Failed to load sound! " << Mix_GetError() << std::endl;
        return false;
    }

    auto sound         = std::make_unique<Sound>();
    sound->m_sdl_chunk = shootSound;

    m_sounds[id] = std::move(sound);

    return true;
}


Texture* AssetManager::getTexture(const std::string& id) const {

    auto it = m_textures.find(id);
    if (it != m_textures.end()) return it->second.get();

    assert(it == m_textures.end());
    return nullptr;
}


Animation* AssetManager::getAnimation(const std::string& id) const {

    auto it = m_animations.find(id);
    if (it != m_animations.end()) return it->second.get();

    assert(it == m_animations.end());
    return nullptr;
}


Sound* AssetManager::getSound(const std::string& id) const {

    auto it = m_sounds.find(id);
    if (it != m_sounds.end()) return it->second.get();

    assert(it == m_sounds.end());
    return nullptr;
}


void AssetManager::loadFont(const std::string& id, const std::string& path) { m_font_paths[id] = path; }

Font* AssetManager::getFont(const std::string& id, std::size_t size) {

    std::string key = id + "_" + std::to_string(size);

    auto it = m_loaded_fonts.find(key);
    if (it != m_loaded_fonts.end()) {
        return it->second.get();
    }


    auto pathIt = m_font_paths.find(id);
    if (pathIt == m_font_paths.end()) {
        printf("ERROR: Font ID '%s' not found in registry!\n", id.c_str());
        return nullptr;
    }

    std::string path     = pathIt->second;
    TTF_Font*   raw_font = TTF_OpenFont(path.c_str(), static_cast<int>(size));

    if (!raw_font) {
        printf("Error loading font file '%s': %s\n", path.c_str(), TTF_GetError());
        return nullptr;
    }

    auto  new_font = std::make_unique<Font>(raw_font, path, static_cast<int>(size));
    Font* ptr      = new_font.get();

    m_loaded_fonts[key] = std::move(new_font);

    return ptr;
}
}  // namespace Engine::Assets