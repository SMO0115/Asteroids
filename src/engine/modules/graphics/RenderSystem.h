//
// Created by marcel on 8/21/25.
//

#pragma once
#include <SDL_ttf.h>
#include <glm/vec2.hpp>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "engine/modules/assets/Texture.h"  // TextureDeleter


struct SDL_Window;
struct SDL_Renderer;

namespace Engine::Core   { class GameObject; }
namespace Engine::Core   { struct TransformComponent; }  // struct, not class: must match the definition (MSVC mangles them differently)
namespace Engine::Core   { struct Rect; }
namespace Engine::Core   { struct Color; }
namespace Engine::Assets { class AssetManager; }


namespace Engine::Graphics {

    struct                                  SpriteComponent;
    struct                                  UITextComponent;

    class RenderSystem {

    public:
                                            RenderSystem();
                                            ~RenderSystem();

        bool                                init(const std::string& title, std::size_t width, std::size_t height);
        void                                beginFrame()                                                                                        const; // Clears the screen
        void                                endFrame();                                                                                                // Presents the rendered frame

        void                                renderWorld( const std::vector<std::unique_ptr<Core::GameObject>>   &game_objects,
                                                         const Assets::AssetManager                             &asset_manager )                const;
        void                                renderUI(    const std::vector<std::unique_ptr<Core::GameObject>>   &ui_objects,
                                                         const Assets::AssetManager                             &asset_manager )                const;

        void                                draw(const SpriteComponent& sprite, const Core::TransformComponent& transform, const Assets::AssetManager& asset_manager)                      const;
        void                                drawRect(const Core::Rect& rect, const Core::Color& color)                                          const;
        void                                drawCircle(int centerX, int centerY, int outerRadius, int innerRadius, const Core::Color& color)    const;
        void                                drawLine(int x1, int y1, int x2, int y2, const Core::Color& color)                                  const;
        void                                drawText(const UITextComponent& text,  const Assets::AssetManager& asset_manager)                   const;
        void                                getTextSize(const std::string& text, int* width, int* height, TTF_Font *font)                       const;

        SDL_Renderer*                       getSDLRenderer() const { return m_renderer; }

        // The size of the game's world on screen, in pixels: the one passed to init(). Ask for it
        // instead of writing 800 somewhere.
        glm::ivec2                          getScreenSize()  const { return m_screen_size; }

    private:
        // A text that drawText() already turned into a texture. Rendering text (SDL_ttf) and uploading
        // it to the GPU is slow; drawing a ready texture is cheap. The HUD and the menus draw the same
        // texts every frame, so they're made once and reused.
        struct CachedText {
            std::unique_ptr<SDL_Texture, Assets::TextureDeleter> texture;
            int                                                  width           = 0;
            int                                                  height          = 0;
            std::uint64_t                                        last_used_frame = 0;
        };

        static SDL_Color                    toSDLColor_(const Core::Color& color);
        void                                setDrawColor_(const Core::Color& color)                                                             const;

        SDL_Window*                         m_window      = nullptr;
        SDL_Renderer*                       m_renderer    = nullptr;
        glm::ivec2                          m_screen_size = {0, 0};

        // Key: font + color + text, everything that changes the picture (not the position: one texture
        // can be drawn anywhere). 'mutable', because drawText() is const: drawing doesn't change what
        // the renderer IS, and filling a cache is the textbook case where a const function may still
        // change a member.
        mutable std::unordered_map<std::string, CachedText> m_text_cache;
        std::uint64_t                                       m_frame = 0;  // counts endFrame() calls
    };
}