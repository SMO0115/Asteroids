//
// Created by marcel on 8/21/25.
//

#pragma once
#include <SDL_ttf.h>
#include <memory>
#include <unordered_map>
#include <vector>

struct SDL_Window;
struct SDL_Renderer;

namespace Engine::Core {
class GameObject;
}
namespace Engine::Core {
class TransformComponent;
}

namespace Engine::Graphics {
class RenderSystem {
public:
    RenderSystem();
    ~RenderSystem();

    bool init(const std::string& title, std::size_t width, std::size_t height);
    void beginFrame() const;  // Clears the screen
    void endFrame() const;    // Presents the rendered frame

    void renderUI(const std::vector<std::unique_ptr<Core::GameObject>>& ui_objects) const;
    void renderWorld(const std::vector<std::unique_ptr<Core::GameObject>>& game_objects) const;

    void draw(const SpriteComponent& sprite, const Core::TransformComponent& transform) const;
    void drawRect(const Core::Rect& rect, const Core::Color& color) const;
    void drawCircle(int centerX, int centerY, int outerRadius, int innerRadius, const Core::Color& color) const;
    void drawLine(int x1, int y1, int x2, int y2, const Core::Color& color) const;
    void drawText(const UITextComponent& text) const;
    void getTextSize(const std::string& text, int* width, int* height, TTF_Font* font) const;

    SDL_Renderer* getSDLRenderer() const { return m_renderer; }

private:
    SDL_Window*   m_window   = nullptr;
    SDL_Renderer* m_renderer = nullptr;
};
}  // namespace Engine::Graphics