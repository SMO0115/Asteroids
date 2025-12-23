//
// Created by marcel on 10/10/25.
//

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

// #include "engine/modules/rendering/AnimationComponent.h"

namespace Engine::Graphics {
class RenderSystem;
}

namespace Engine::Assets {

struct Font;
struct Texture;
struct Animation;
struct Sound;

class AssetManager {
public:
    AssetManager()  = default;
    ~AssetManager() = default;

    bool loadTexture(Graphics::RenderSystem& renderer, const std::string& id, const std::string& path);
    bool loadAnimation(const std::string& id, const std::string& path);
    bool loadSound(const std::string& id, const std::string& path);
    void loadFont(const std::string& id, const std::string& path);

    Texture*   getTexture(const std::string& id) const;
    Animation* getAnimation(const std::string& id) const;
    Sound*     getSound(const std::string& id) const;
    Font*      getFont(const std::string& id, std::size_t size);

private:
    std::unordered_map<std::string, std::unique_ptr<Texture>>   m_textures;
    std::unordered_map<std::string, std::unique_ptr<Animation>> m_animations;
    std::unordered_map<std::string, std::unique_ptr<Sound>>     m_sounds;
    std::unordered_map<std::string, std::string>                m_font_paths;
    std::unordered_map<std::string, std::unique_ptr<Font>>      m_loaded_fonts;
};
}  // namespace Engine::Assets