#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include "engine/utils/Types.h"

namespace Engine::Graphics { class RenderSystem; }

namespace Engine::Assets {

    struct Font;
    struct Texture;
    struct Animation;
    struct Sound;


    class AssetManager {
    public:
        AssetManager(Graphics::RenderSystem* renderer);
        ~AssetManager() = default;

        // --- 1. Load (Returns ID) ---
        // Calling this with the same id_string twice returns the SAME numeric ID.
        AssetID loadTexture(const std::string& name, const std::string& path);
        AssetID loadAnimation(const std::string& name, const std::string& path);
        AssetID loadSound(const std::string& name, const std::string& path);
        
        // Fonts need size to be unique resources
        AssetID loadFont(const std::string& name, const std::string& path, int size);

        // --- 2. Retrieve (Fast Array Access) ---
        Texture* getTexture(AssetID id) const;
        Animation* getAnimation(AssetID id) const;
        Sound* getSound(AssetID id) const;
        Font* getFont(AssetID id) const;

        // --- 3. Helper (String -> ID) ---
        AssetID getTextureID(const std::string& name) const;
        AssetID getAnimationID(const std::string& name) const;
        AssetID getSoundID(const std::string& name) const;
        AssetID getFontID(const std::string& name, int size) const;

    private:
        Graphics::RenderSystem* m_renderer;

        // STORAGE (Contiguous memory = Fast)
        // We use index 0 as a dummy/fallback so 0 always means "Invalid".
        std::vector<std::unique_ptr<Texture>>   m_textures;
        std::vector<std::unique_ptr<Animation>> m_animations;
        std::vector<std::unique_ptr<Sound>>     m_sounds;
        std::vector<std::unique_ptr<Font>>      m_fonts;

        // LOOKUP (String -> Index)
        std::unordered_map<std::string, AssetID> m_texture_lookup;
        std::unordered_map<std::string, AssetID> m_animation_lookup;
        std::unordered_map<std::string, AssetID> m_sound_lookup;
        std::unordered_map<std::string, AssetID> m_font_lookup; // Key example: "pixel_24"
    };
}