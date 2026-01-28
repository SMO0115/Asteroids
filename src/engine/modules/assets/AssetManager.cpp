#include "engine/modules/assets/AssetManager.h"

#include <cassert>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h" // Don't forget this for TTF_OpenFont

#include "yyjson.h"

#include "engine/modules/assets/Animation.h"
#include "engine/modules/assets/Font.h"
#include "engine/modules/assets/Sound.h"
#include "engine/modules/assets/Texture.h"

#include "engine/modules/graphics/RenderModule.h"

namespace Engine::Assets {

    AssetManager::AssetManager(Graphics::RenderSystem* renderer) : m_renderer(renderer) {

        m_textures.push_back(nullptr);
        m_animations.push_back(nullptr);
        m_sounds.push_back(nullptr);
        m_fonts.push_back(nullptr);
    }


    AssetID AssetManager::loadTexture(const std::string& name, const std::string& path) {

        if (m_texture_lookup.contains(name)) return m_texture_lookup[name];

        SDL_Surface* temp_surface = IMG_Load(path.c_str());
        if (temp_surface == nullptr) {
            std::cerr << "[AssetManager] Failed to load image: " << path << " | " << IMG_GetError() << std::endl;
            return INVALID_ID;
        }

        SDL_Texture* sdl_texture = SDL_CreateTextureFromSurface(m_renderer->getSDLRenderer(), temp_surface);

        int w = temp_surface->w;
        int h = temp_surface->h;
        SDL_FreeSurface(temp_surface);

        if (sdl_texture == nullptr) {
            std::cerr << "[AssetManager] Failed to create texture: " << SDL_GetError() << std::endl;
            return INVALID_ID;
        }

        auto engine_texture = std::make_unique<Texture>();
        engine_texture->texture = sdl_texture;
        engine_texture->width   = w;
        engine_texture->height  = h;

        AssetID newId = static_cast<AssetID>(m_textures.size());
        m_textures.push_back(std::move(engine_texture));
        m_texture_lookup[name] = newId;

        return newId;
    }

    Texture* AssetManager::getTexture(AssetID id) const {
        if (id > 0 && id < m_textures.size()) {
            return m_textures[id].get();
        }
        return nullptr; // Return m_textures[0] (fallback) here later
    }

    AssetID AssetManager::getTextureID(const std::string& name) const {
        auto it = m_texture_lookup.find(name);
        return (it != m_texture_lookup.end()) ? it->second : INVALID_ID;
    }


    AssetID AssetManager::loadAnimation(const std::string& name, const std::string& path) {
        if (m_animation_lookup.contains(name)) return m_animation_lookup[name];

        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "[AssetManager] Error: Could not open JSON: " << path << std::endl;
            return INVALID_ID;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string fileContent = buffer.str();

        yyjson_doc* doc = yyjson_read(fileContent.data(), fileContent.length(), 0);
        if (!doc) {
            std::cerr << "[AssetManager] Error: Failed to parse JSON." << std::endl;
            return INVALID_ID;
        }

        yyjson_val* root   = yyjson_doc_get_root(doc);
        yyjson_val* frames = yyjson_obj_get(root, "frames");


        auto animation = std::make_unique<Animation>();
        animation->texture_id   = getTextureID(name);
        animation->loop         = true;

        size_t      idx, max;
        yyjson_val* frame;
        const float DEFAULT_DURATION = 0.1f; // Changed to 0.1s (typical 10fps)

        yyjson_arr_foreach(frames, idx, max, frame) {

            yyjson_val* frame_obj = yyjson_obj_get(frame, "frame");
            const int x = yyjson_get_int(yyjson_obj_get(frame_obj, "x"));
            const int y = yyjson_get_int(yyjson_obj_get(frame_obj, "y"));
            const int w = yyjson_get_int(yyjson_obj_get(frame_obj, "w"));
            const int h = yyjson_get_int(yyjson_obj_get(frame_obj, "h"));

            float duration = DEFAULT_DURATION;
            yyjson_val* duration_val = yyjson_obj_get(frame, "duration");
            if (duration_val && yyjson_is_real(duration_val)) {
                duration = (float)yyjson_get_real(duration_val) / 1000.0f; // Assuming JSON is in ms
            }

            animation->frames.push_back({x, y, w, h});
            animation->durations.push_back(duration);
        }

        AssetID newId = static_cast<AssetID>(m_animations.size());
        m_animations.push_back(std::move(animation));
        m_animation_lookup[name] = newId;

        yyjson_doc_free(doc);
        return newId;
    }

    Animation* AssetManager::getAnimation(AssetID id) const {
        if (id > 0 && id < m_animations.size()) return m_animations[id].get();
        return nullptr;
    }

    AssetID AssetManager::getAnimationID(const std::string& name) const {
        auto it = m_animation_lookup.find(name);
        return (it != m_animation_lookup.end()) ? it->second : INVALID_ID;
    }



    AssetID AssetManager::loadSound(const std::string& name, const std::string& path) {
        if (m_sound_lookup.contains(name)) return m_sound_lookup[name];

        Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
        if (!chunk) {
            std::cerr << "[AssetManager] Failed to load sound: " << Mix_GetError() << std::endl;
            return INVALID_ID;
        }
        Mix_VolumeChunk(chunk, 10); // Default volume

        auto sound = std::make_unique<Sound>();
        sound->m_sdl_chunk = chunk;

        AssetID newId = static_cast<AssetID>(m_sounds.size());
        m_sounds.push_back(std::move(sound));
        m_sound_lookup[name] = newId;

        return newId;
    }

    Sound* AssetManager::getSound(AssetID id) const {
        if (id > 0 && id < m_sounds.size()) return m_sounds[id].get();
        return nullptr;
    }

    AssetID AssetManager::getSoundID(const std::string& name) const {
        auto it = m_sound_lookup.find(name);
        return (it != m_sound_lookup.end()) ? it->second : INVALID_ID;
    }



    AssetID AssetManager::loadFont(const std::string& name, const std::string& path, int size) {
        // Unique key for this specific size variant: "pixel_24"
        std::string key = name + "_" + std::to_string(size);

        if (m_font_lookup.contains(key)) return m_font_lookup[key];

        TTF_Font* raw_font = TTF_OpenFont(path.c_str(), size);
        if (!raw_font) {
            std::cerr << "[AssetManager] Error loading font '" << path << "': " << TTF_GetError() << std::endl;
            return INVALID_ID;
        }

        // Create wrapper
        // Note: Ensure your Font struct constructor takes (TTF_Font*, path, size)
        auto new_font = std::make_unique<Font>(raw_font, path, size);

        AssetID newId = static_cast<AssetID>(m_fonts.size());
        m_fonts.push_back(std::move(new_font));
        m_font_lookup[key] = newId;

        return newId;
    }

    Font* AssetManager::getFont(AssetID id) const {
        if (id > 0 && id < m_fonts.size()) return m_fonts[id].get();
        return nullptr;
    }

    AssetID AssetManager::getFontID(const std::string& name, int size) const {
        std::string key = name + "_" + std::to_string(size);
        auto it = m_font_lookup.find(key);
        return (it != m_font_lookup.end()) ? it->second : INVALID_ID;
    }

} // namespace Engine::Assets