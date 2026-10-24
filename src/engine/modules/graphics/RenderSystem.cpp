//
// Created by marcel on 8/21/25.
//

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "RenderSystem.h"
#include "SpriteComponent.h"
#include "UITextComponent.h"

#include "engine/core/CoreModule.h"
#include "engine/core/Log.h"
#include "engine/modules/assets/AssetModule.h"


namespace Engine::Graphics {
RenderSystem::RenderSystem() = default;
RenderSystem::~RenderSystem() {

    // The cached textures belong to the renderer, so they must go BEFORE it. (Members are destroyed
    // after the destructor's body, which would be too late.)
    m_text_cache.clear();

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);


    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool RenderSystem::init(const std::string& title, std::size_t width, std::size_t height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ENGINE_FATAL("[Renderer] Failed to initialize SDL Video: {}", SDL_GetError());
        return false;
    }
    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(width), static_cast<int>(height), 0);
    if (!m_window) {
        ENGINE_FATAL("[Renderer] Failed to create window: {}", SDL_GetError());
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Check BEFORE using it: SDL_GetRendererInfo(nullptr, ...) would fail and leave 'info' uninitialized
    if (!m_renderer) {
        ENGINE_FATAL("[Renderer] Failed to create renderer: {}", SDL_GetError());
        return false;
    }

    SDL_RendererInfo info;
    SDL_GetRendererInfo(m_renderer, &info);
    ENGINE_INFO("[Renderer] Name: {}, accelerated: {}", info.name, (info.flags & SDL_RENDERER_ACCELERATED) ? "yes" : "no");

    // The game always draws in this size, even if the window ends up bigger or smaller (a tiling
    // window manager may resize it): SDL then scales the picture and adds black bars
    m_screen_size = {static_cast<int>(width), static_cast<int>(height)};
    SDL_RenderSetLogicalSize(m_renderer, m_screen_size.x, m_screen_size.y);

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        ENGINE_ERROR("[Renderer] IMG_Init: {}", IMG_GetError());
    }


    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    if (TTF_Init() == -1) {
        ENGINE_FATAL("[Renderer] TTF_Init: {}", TTF_GetError());
        return false;
    }

    return true;
}

void RenderSystem::beginFrame() const {

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
}

void RenderSystem::endFrame() {

    SDL_RenderPresent(m_renderer);

    // Forget every text that wasn't drawn in this frame (e.g. the old score, once it changed), so the
    // cache only ever holds what's on screen and can't grow without limit
    std::erase_if(m_text_cache, [this](const auto& entry) { return entry.second.last_used_frame != m_frame; });
    ++m_frame;
}

void RenderSystem::renderUI(const std::vector<std::unique_ptr<Core::GameObject> >& ui_objects, const Assets::AssetManager& asset_manager) const {

    for (const auto& object : ui_objects) {

        if (!object->isActive()) continue;

        UITextComponent& text = object->getComponent<UITextComponent>();
        drawText(text, asset_manager);
    }
}


void RenderSystem::renderWorld(const std::vector<std::unique_ptr<Core::GameObject> >& game_objects, const Assets::AssetManager& asset_manager) const {

    for (auto& object : game_objects) {

        if (!object->isActive()) continue;
        if (!object->hasComponent<SpriteComponent>()) continue;
        if (!object->hasComponent<Core::TransformComponent>()) continue;

        SpriteComponent&          sprite    = object->getComponent<SpriteComponent>();
        Core::TransformComponent& transform = object->getComponent<Core::TransformComponent>();

        draw(sprite, transform, asset_manager);
    }
}


void RenderSystem::draw(const SpriteComponent& sprite, const Core::TransformComponent& transform, const Assets::AssetManager& asset_manager) const {

    if (sprite.texture_id == 0) return;

    const Assets::Texture* texture = asset_manager.getTexture(sprite.texture_id);
    if (texture == nullptr) return;  // stale/unknown ID: skip instead of dereferencing nullptr

    SDL_Rect srcRect = {sprite.sourceRect.x, sprite.sourceRect.y, sprite.sourceRect.w, sprite.sourceRect.h};

    float scaledWidth  = static_cast<float>(sprite.sourceRect.w) * transform.scale;
    float scaledHeight = static_cast<float>(sprite.sourceRect.h) * transform.scale;

    int destX = static_cast<int>(transform.position.x - (scaledWidth / 2.0f));
    int destY = static_cast<int>(transform.position.y - (scaledHeight / 2.0f));

    SDL_Rect destRect = {destX, destY, static_cast<int>(scaledWidth), static_cast<int>(scaledHeight)};


    SDL_RenderCopyEx(m_renderer, texture->texture.get(), &srcRect, &destRect,
                     static_cast<double>(transform.rotation),  // Angle in degrees
                     nullptr,                                  // Center of rotation: NULL defaults to the center of destRect
                     SDL_FLIP_NONE                             // Flip: SDL_FLIP_HORIZONTAL, SDL_FLIP_VERTICAL, or SDL_FLIP_NONE
    );
}


void RenderSystem::drawRect(const Core::Rect& rect, const Core::Color& color) const {

    setDrawColor_(color);
    SDL_Rect sdl_rect = {rect.x, rect.y, rect.w, rect.h};
    SDL_RenderFillRect(m_renderer, &sdl_rect);
}


void RenderSystem::drawCircle(int centerX, int centerY, int outerRadius, int innerRadius, const Core::Color& color) const {

    const int outerRadiusSq = outerRadius * outerRadius;
    const int innerRadiusSq = innerRadius * innerRadius;

    setDrawColor_(color);  // was missing: the circle used whatever color was set last


    for (int y = -outerRadius; y <= outerRadius; ++y) {
        for (int x = -outerRadius; x <= outerRadius; ++x) {

            int distanceSq = x * x + y * y;

            if (distanceSq >= innerRadiusSq && distanceSq <= outerRadiusSq) SDL_RenderDrawPoint(m_renderer, centerX + x, centerY + y);
        }
    }
}


void RenderSystem::drawLine(int x1, int y1, int x2, int y2, const Core::Color& color) const {

    setDrawColor_(color);
    SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
}


void RenderSystem::drawText(const UITextComponent& text, const Assets::AssetManager& asset_manager) const {


    const Assets::Font* font = asset_manager.getFont(text.font_id);
    if (font == nullptr || text.text.empty()) return;  // SDL_ttf can't render an empty string

    const Core::Color& c   = text.color;
    const std::string  key = std::to_string(text.font_id) + '|' + std::to_string(c.r) + ',' + std::to_string(c.g) + ',' + std::to_string(c.b) + ',' +
                             std::to_string(c.a) + '|' + text.text;

    auto cached = m_text_cache.find(key);
    if (cached == m_text_cache.end()) {

        // Not drawn last frame: render it once (this used to happen for every text, every frame)
        SDL_Surface* surface = TTF_RenderText_Solid(font->get(), text.text.c_str(), toSDLColor_(c));
        if (!surface) return;

        CachedText entry;
        entry.texture.reset(SDL_CreateTextureFromSurface(m_renderer, surface));
        entry.width  = surface->w;
        entry.height = surface->h;
        SDL_FreeSurface(surface);
        if (!entry.texture) return;

        cached = m_text_cache.emplace(key, std::move(entry)).first;
    }

    cached->second.last_used_frame = m_frame;  // still on screen: keep it

    const SDL_Rect dest_rect = {text.x_offset, text.y_offset, cached->second.width, cached->second.height};
    SDL_RenderCopy(m_renderer, cached->second.texture.get(), nullptr, &dest_rect);
}


void RenderSystem::getTextSize(const std::string& text, int* width, int* height, TTF_Font* font) const {

    assert(font != nullptr);

    TTF_SizeText(font, text.c_str(), width, height);
}


// Core::Color stores int channels, SDL wants 0..255 bytes: convert explicitly, in one place
SDL_Color RenderSystem::toSDLColor_(const Core::Color& color) {
    return {static_cast<Uint8>(color.r), static_cast<Uint8>(color.g), static_cast<Uint8>(color.b), static_cast<Uint8>(color.a)};
}

void RenderSystem::setDrawColor_(const Core::Color& color) const {
    const SDL_Color c = toSDLColor_(color);
    SDL_SetRenderDrawColor(m_renderer, c.r, c.g, c.b, c.a);
}
}  // namespace Engine::Graphics