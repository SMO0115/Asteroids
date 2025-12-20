//
// Created by marcel on 8/21/25.
//

#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "SpriteComponent.h"
#include "UITextComponent.h"
#include "RenderSystem.h"
#include <SDL_ttf.h>
#include <cassert>

#include "engine/core/CoreModule.h"
#include "engine/modules/assets/AssetModule.h"


RenderSystem::RenderSystem() = default;
RenderSystem::~RenderSystem() {

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);


    TTF_Quit();
    SDL_Quit();
}

bool RenderSystem::init(const std::string &title, std::size_t width, std::size_t height) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL Video: " << SDL_GetError() << std::endl;
        return false;
    }

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!m_window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
    }


    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

void RenderSystem::beginFrame() const {

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
}

void RenderSystem::endFrame() const {

    SDL_RenderPresent(m_renderer);
}

void RenderSystem::renderUI( const std::vector<std::unique_ptr<GameObject> > &ui_objects ) const {

    for (const auto &object : ui_objects) {

        if (!object->isActive()) continue;

        UITextComponent& text = object->getComponent<UITextComponent>();
        drawText(text);
    }
}


void RenderSystem::renderWorld(const std::vector<std::unique_ptr<GameObject> > &game_objects) const {

    for (auto &object : game_objects) {

        if (!object->isActive()) continue;

        SpriteComponent& sprite = object->getComponent<SpriteComponent>();
        TransformComponent& transform = object->getComponent<TransformComponent>();

        draw(sprite, transform);
    }

}



void RenderSystem::draw(const SpriteComponent &sprite, const TransformComponent &transform) const {


    if (sprite.texture == nullptr) return;

    SDL_Rect srcRect = {
        sprite.sourceRect.x,
        sprite.sourceRect.y,
        sprite.sourceRect.w,
        sprite.sourceRect.h
    };

    float scaledWidth  = sprite.sourceRect.w * transform.scale;
    float scaledHeight = sprite.sourceRect.h * transform.scale;

    int destX = static_cast<int>(transform.position.x - (scaledWidth / 2.0f));
    int destY = static_cast<int>(transform.position.y - (scaledHeight / 2.0f));

    SDL_Rect destRect = {
        destX,
        destY,
        static_cast<int>(scaledWidth),
        static_cast<int>(scaledHeight)
    };

    SDL_RenderCopy(m_renderer, sprite.texture->texture, &srcRect, &destRect);
}




void RenderSystem::drawRect(const Rect& rect, const Color& color) const {

    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_Rect sdl_rect = { rect.x, rect.y, rect.w, rect.h };
    SDL_RenderFillRect(m_renderer, &sdl_rect);
}


void RenderSystem::drawCircle(int centerX, int centerY, int outerRadius, int innerRadius, const Color& color) const {

    const int outerRadiusSq = outerRadius * outerRadius;
    const int innerRadiusSq = innerRadius * innerRadius;


    for (int y = -outerRadius; y <= outerRadius; ++y) {
        for (int x = -outerRadius; x <= outerRadius; ++x) {

            int distanceSq = x * x + y * y;

            if (distanceSq >= innerRadiusSq && distanceSq <= outerRadiusSq)
                SDL_RenderDrawPoint(m_renderer, centerX + x, centerY + y);

        }
    }

}


void RenderSystem::drawLine(int x1, int y1, int x2, int y2, const Color& color) const {

    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
}





void RenderSystem::drawText(const UITextComponent& text) const {


    auto font = text.font;
    if (!font) return;

    SDL_Color sdl_color = {text.color.r, text.color.g, text.color.b, 255};
    SDL_Surface* text_surface = TTF_RenderText_Solid(text.font->ttf_font.get(), text.text.c_str(), sdl_color);
    if (!text_surface) return;

    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
    SDL_FreeSurface(text_surface);
    if (!text_texture) return;

    SDL_Rect dest_rect = {text.x_offset, text.y_offset, 0, 0};
    SDL_QueryTexture(text_texture, nullptr, nullptr, &dest_rect.w, &dest_rect.h);
    SDL_RenderCopy(m_renderer, text_texture, nullptr, &dest_rect);

    SDL_DestroyTexture(text_texture);
}


void RenderSystem::getTextSize(const std::string& text, int* width, int* height, TTF_Font *font) const {

    assert(font != nullptr);

    TTF_SizeText(font, text.c_str(), width, height);
}
