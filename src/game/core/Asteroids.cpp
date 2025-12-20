//
// Created by marcel on 9/28/25.
//

#include "glm/glm.hpp"


#include "Asteroids.h"

#include "engine/core/CoreModule.h"

#include "engine/modules/rendering/RenderModule.h"
#include "engine/modules/physics/PhysicsModule.h"
#include "engine/modules/audio/AudioModule.h"
#include "engine/modules/assets/AssetModule.h"

#include "game/modules/control/ControlModul.h"
#include "game/modules/emitter/EmiterModule.h"
#include "game/modules/health/HealthModule.h"
#include "game/modules/ui/UIModule.h"

#include "GameTypes.h"

#include "../modules/control/InvaderComponent.h"
#include "../modules/control/PlayerComponent.h"

#include "tags/Tags.h"
#include "tags/WallComponent.h"

#include "game/events/GameEvents.h"

#include "../states/RunState.h"


Asteroids::Asteroids() {

    m_player_control_system = std::make_unique<PlayerControlSystem>();
    m_ai_system             = std::make_unique<AISystem>();
    m_health_system         = std::make_unique<HealthSystem>();
    m_lifetime_system       = std::make_unique<LifetimeSystem>();
    m_score_system          = std::make_unique<ScoreSystem>();
    m_ui_system             = std::make_unique<UISystem>();
    m_emitter_system        = std::make_unique<EmitterSystem>();
    m_event_bus             = std::make_unique<GameEventBus>();
}

Asteroids::~Asteroids() {

}


bool Asteroids::init(Engine &engine) {

    m_game_session = std::make_unique<GameObject>();
    m_game_session->addComponent<GameStateComponent>(0, 3, 55);




    AssetManager& asset_manager = engine.getAssetManager();
    RenderSystem& renderer = engine.getRenderer();

    try {
        // entity textures
        asset_manager.loadTexture(renderer, "invader_A", "../assets/Sprites/invader_A.png");
        asset_manager.loadAnimation("invader_A", "../assets/Sprites/invader_A.json");

        asset_manager.loadTexture(renderer, "invader_B", "../assets/Sprites/invader_B.png");
        asset_manager.loadAnimation("invader_B", "../assets/Sprites/invader_B.json");

        asset_manager.loadTexture(renderer, "invader_C", "../assets/Sprites/invader_C.png");
        asset_manager.loadAnimation("invader_C", "../assets/Sprites/invader_C.json");

        asset_manager.loadTexture(renderer, "player", "../assets/Sprites/player.png");
        asset_manager.loadAnimation("player", "../assets/Sprites/player.json");

        asset_manager.loadTexture(renderer, "UFO", "../assets/Sprites/UFO.png");
        asset_manager.loadAnimation("UFO", "../assets/Sprites/UFO.json");


        asset_manager.loadTexture(renderer, "player_death", "../assets/Sprites/player_death.png");
        asset_manager.loadAnimation("player_death", "../assets/Sprites/player_death.json");

        asset_manager.loadTexture(renderer, "invader_death", "../assets/Sprites/invader_death.png");
        asset_manager.loadAnimation("invader_death", "../assets/Sprites/invader_death.json");


        for (int k = 0; k < 4; k++){
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++){
                    std::string wall_id = "wall_" + std::to_string(k) + "_" + std::to_string(i) + "_" + std::to_string(j);
                    asset_manager.loadTexture(renderer, wall_id, "../assets/Sprites/wall.png");
                    asset_manager.loadAnimation(wall_id, "../assets/Sprites/wall.json");

                    std::string wall_destruction_id = "wall_destruction_" + std::to_string(k) + "_" + std::to_string(i) + "_" + std::to_string(j);
                    asset_manager.loadTexture(renderer, wall_destruction_id, "../assets/Sprites/destruction.png");
                    asset_manager.loadAnimation(wall_destruction_id, "../assets/Sprites/invader_death.json");
                }
            }
        }



        asset_manager.loadTexture(renderer, "invader_death", "../assets/Sprites/invader_death.png");
        asset_manager.loadAnimation("invader_death", "../assets/Sprites/invader_death.json");


        // projectiles
        asset_manager.loadTexture(renderer, "missile", "../assets/Sprites/missile.png");
        asset_manager.loadAnimation("missile", "../assets/Sprites/missile.json");

        asset_manager.loadTexture(renderer, "projectile_A", "../assets/Sprites/projectile_A.png");
        asset_manager.loadAnimation("projectile_A", "../assets/Sprites/projectile_A.json");

        asset_manager.loadTexture(renderer, "projectile_B", "../assets/Sprites/projectile_B.png");
        asset_manager.loadAnimation("projectile_B", "../assets/Sprites/projectile_B.json");

        asset_manager.loadTexture(renderer, "projectile_C", "../assets/Sprites/projectile_C.png");
        asset_manager.loadAnimation("projectile_C", "../assets/Sprites/projectile_C.json");

        asset_manager.loadTexture(renderer, "player_missile", "../assets/Sprites/player_missile.png");
        asset_manager.loadAnimation("player_missile", "../assets/Sprites/player_missile.json");


        // sounds
        asset_manager.loadSound("explosion"     , "../assets/Audio/explosion.wav");
        asset_manager.loadSound("player_shoot"  , "../assets/Audio/shoot.wav");
        asset_manager.loadSound("fastinvader1"  , "../assets/Audio/fastinvader1.wav");
        asset_manager.loadSound("fastinvader2"  , "../assets/Audio/fastinvader2.wav");
        asset_manager.loadSound("fastinvader3"  , "../assets/Audio/fastinvader3.wav");
        asset_manager.loadSound("fastinvader4"  , "../assets/Audio/fastinvader4.wav");
        asset_manager.loadSound("ufo_highpitch" , "../assets/Audio/ufo_highpitch.wav");
        asset_manager.loadSound("ufo_lowpitch"  , "../assets/Audio/ufo_lowpitch.wav");
        asset_manager.loadSound("shoot"         , "../assets/Audio/shoot.wav");


        asset_manager.loadFont("pixel", "../assets/Fonts/PixelifySans.ttf");


        //asset_manager.loadSpriteDefinitions("assets/spritesheet.json");
        // asset_manager.loadSound("player_shoot", "assets/sounds/shoot.wav");
        // asset_manager.loadSound("explosion", "assets/sounds/explosion.wav");
    } catch (const std::exception& e) {
        return false;
    }

    {
        std::unique_ptr<GameObject> UI_Score = std::make_unique<GameObject>();

        Font* font = asset_manager.getFont("pixel",24);
        UI_Score->addComponent<UITextComponent>(
            "Score: " + std::to_string(0),
            std::move(font),
            Color{255, 255, 255}, 20, 10
            );
        UI_Score->addComponent<ScoreDisplayTag>();
        m_ui_objects.push_back(std::move(UI_Score));
    }
    {
        std::unique_ptr<GameObject> UI_Lives = std::make_unique<GameObject>();

        Font* font = asset_manager.getFont("pixel",24);
        UI_Lives->addComponent<UITextComponent>(
            "Lives: " + std::to_string(0),
            std::move(font),
            Color{255, 255, 255}, 690, 10
            );
        UI_Lives->addComponent<LivesDisplayTag>();
        m_ui_objects.push_back(std::move(UI_Lives));
    }



    {
        std::unique_ptr<GameObject> player = std::make_unique<GameObject>();

        AnimationComponent animation_component;
        animation_component.animations[ static_cast<int>( AnimationState::IDLE ) ] = asset_manager.getAnimation("player");
        animation_component.animations[ static_cast<int>( AnimationState::DEATH ) ] = asset_manager.getAnimation("player_death");
        animation_component.current_state = static_cast<int>( AnimationState::IDLE );



        ColliderComponent collider_component;
        collider_component.bounds.w = animation_component.animations[ static_cast<int>( AnimationState::IDLE ) ]->frames[0].w;
        collider_component.bounds.h = animation_component.animations[ static_cast<int>( AnimationState::IDLE ) ]->frames[0].h;
        collider_component.layer = CollisionLayer::PLAYER;
        collider_component.mask = CollisionLayer::INVADER | CollisionLayer::INVADER_BULLET;
        player->addComponent<ColliderComponent>(std::move(collider_component));
        player->addComponent<AnimationComponent>(std::move(animation_component));

        SoundComponent sound_component;
        sound_component.sounds[ static_cast<int>( SoundState::IDLE ) ] = "fastinvader1"; //asset_manager.getSound("fastinvader1");
        sound_component.current_state = static_cast<int>( SoundState::IDLE );
        player->addComponent<SoundComponent>(std::move(sound_component));


        player->addComponent<TransformComponent>();
        player->getComponent<TransformComponent>().position = glm::vec2(400, 750);
        player->getComponent<TransformComponent>().scale = 2;
        player->addComponent<PhysicsComponent>();
        player->addComponent<SpriteComponent>();
        player->addComponent<InputComponent>();
        player->addComponent<PlayerComponent>();

        player->addComponent<HealthComponent>();
        player->getComponent<HealthComponent>().max_health = 100;
        player->getComponent<HealthComponent>().current_health = 100;

        m_objects.push_back(std::move(player));
    }


    for (int k = 0; k < 4; k++) {

        glm::vec2 wall_offset(100 + k * 180, 630);

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {

                {
                    std::unique_ptr<GameObject> wall = std::make_unique<GameObject>();
                    std::string wall_id = "wall_" + std::to_string(k) + "_" + std::to_string(i) + "_" + std::to_string(j);

                    AnimationComponent animation_component;
                    animation_component.animations[ static_cast<int>( AnimationState::IDLE ) ] = asset_manager.getAnimation(wall_id);
                    animation_component.animations[ static_cast<int>( AnimationState::IDLE ) ]->frames[0] = {11*i, 8*j, 11, 8};
                    animation_component.animations[ static_cast<int>( AnimationState::DEATH ) ] = nullptr;
                    animation_component.current_state = static_cast<int>( AnimationState::IDLE );

                    //wall->addComponent<WallComponent>();

                    ColliderComponent collider_component;
                    collider_component.bounds.w = animation_component.animations[ static_cast<int>( AnimationState::IDLE ) ]->frames[0].w;
                    collider_component.bounds.h = animation_component.animations[ static_cast<int>( AnimationState::IDLE ) ]->frames[0].h;
                    collider_component.layer = CollisionLayer::WALL;
                    collider_component.mask = CollisionLayer::INVADER_BULLET | CollisionLayer::PLAYER_BULLET;
                    wall->addComponent<ColliderComponent>(std::move(collider_component));
                    wall->addComponent<AnimationComponent>(std::move(animation_component));

                    // SoundComponent sound_component;
                    // sound_component.sounds[ static_cast<int>( SoundState::IDLE ) ] = "fastinvader1"; //asset_manager.getSound("fastinvader1");
                    // sound_component.current_state = static_cast<int>( SoundState::IDLE );
                    // wall->addComponent<SoundComponent>(std::move(sound_component));


                    wall->addComponent<TransformComponent>();
                    wall->getComponent<TransformComponent>().position = wall_offset + glm::vec2(i*11*2, j*8*2);
                    wall->getComponent<TransformComponent>().scale = 2;
                    wall->addComponent<SpriteComponent>();

                    wall->addComponent<HealthComponent>();
                    wall->getComponent<HealthComponent>().max_health = 400;
                    wall->getComponent<HealthComponent>().current_health = 400;


                    m_objects.push_back(std::move(wall));
                }



                {
                    std::unique_ptr<GameObject> wall = std::make_unique<GameObject>();
                    std::string wall_destruction_id = "wall_destruction_" + std::to_string(k) + "_" + std::to_string(i) + "_" + std::to_string(j);

                    AnimationComponent animation_component;
                    animation_component.animations[ static_cast<int>( AnimationState::IDLE ) ] = asset_manager.getAnimation(wall_destruction_id);
                    animation_component.animations[ static_cast<int>( AnimationState::IDLE ) ]->frames[0] = {0, 0, 11, 8};
                    animation_component.animations[ static_cast<int>( AnimationState::DEATH ) ] = nullptr;
                    animation_component.current_state = static_cast<int>( AnimationState::IDLE );

                    wall->addComponent<WallComponent>();

                    ColliderComponent collider_component;
                    collider_component.bounds.w = animation_component.animations[ static_cast<int>( AnimationState::IDLE ) ]->frames[0].w;
                    collider_component.bounds.h = animation_component.animations[ static_cast<int>( AnimationState::IDLE ) ]->frames[0].h;
                    collider_component.layer = CollisionLayer::WALL;
                    collider_component.mask = CollisionLayer::INVADER_BULLET | CollisionLayer::PLAYER_BULLET;
                    wall->addComponent<ColliderComponent>(std::move(collider_component));
                    wall->addComponent<AnimationComponent>(std::move(animation_component));

                    // SoundComponent sound_component;
                    // sound_component.sounds[ static_cast<int>( SoundState::IDLE ) ] = "fastinvader1"; //asset_manager.getSound("fastinvader1");
                    // sound_component.current_state = static_cast<int>( SoundState::IDLE );
                    // wall->addComponent<SoundComponent>(std::move(sound_component));


                    wall->addComponent<TransformComponent>();
                    wall->getComponent<TransformComponent>().position = wall_offset + glm::vec2(i*11*2, j*8*2);
                    wall->getComponent<TransformComponent>().scale = 2;
                    wall->addComponent<SpriteComponent>();

                    wall->addComponent<HealthComponent>();
                    wall->getComponent<HealthComponent>().max_health = 400;
                    wall->getComponent<HealthComponent>().current_health = 400;

                    m_objects.push_back(std::move(wall));
                }
            }
        }
    }


    std::size_t N = 11;
    std::size_t M = 5;
    std::size_t spacing = 60;
    std::array<std::string, 5> s_enemy_types = {"invader_A", "invader_B", "invader_B", "invader_C", "invader_C"};
    std::array<InvaderType, 5> e_enemy_types = {InvaderType::INVADER_A, InvaderType::INVADER_B, InvaderType::INVADER_B, InvaderType::INVADER_C, InvaderType::INVADER_C};
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {

            std::unique_ptr<GameObject> enemy = std::make_unique<GameObject>();


            AnimationComponent animation_component;
            animation_component.animations[ static_cast<int>( AnimationState::IDLE ) ] = asset_manager.getAnimation(s_enemy_types[j]);
            animation_component.animations[ static_cast<int>( AnimationState::DEATH ) ] = asset_manager.getAnimation("invader_death");
            animation_component.current_state = static_cast<int>( AnimationState::IDLE );
            animation_component.current_frame = j % 2;

            ColliderComponent collider_component;
            collider_component.bounds.w = animation_component.animations[ static_cast<int>( AnimationState::IDLE ) ]->frames[0].w;
            collider_component.bounds.h = animation_component.animations[ static_cast<int>( AnimationState::IDLE ) ]->frames[0].h;
            collider_component.layer = CollisionLayer::INVADER;
            collider_component.mask = CollisionLayer::PLAYER_BULLET | CollisionLayer::PLAYER;
            enemy->addComponent<ColliderComponent>(std::move(collider_component));
            enemy->addComponent<AnimationComponent>(std::move(animation_component));


            SoundComponent sound_component;
            sound_component.sounds[ static_cast<int>( SoundState::IDLE ) ] = "fastinvader1"; //asset_manager.getSound("fastinvader1");
            sound_component.current_state = static_cast<int>( SoundState::IDLE );
            enemy->addComponent<SoundComponent>(std::move(sound_component));

            enemy->addComponent<TransformComponent>();
            enemy->getComponent<TransformComponent>().position = glm::vec2(35 + i * spacing, 100 + j * spacing);
            enemy->getComponent<TransformComponent>().scale = 1.5;

            enemy->addComponent<InvaderComponent>();
            enemy->getComponent<InvaderComponent>().type = e_enemy_types[j];
            enemy->getComponent<InvaderComponent>().score_value = static_cast<int>(e_enemy_types[j]) * (j + 1);

            enemy->addComponent<PhysicsComponent>();
            enemy->addComponent<SpriteComponent>();
            enemy->addComponent<AIComponent>();

            enemy->addComponent<HealthComponent>();
            enemy->getComponent<HealthComponent>().max_health = 100;
            enemy->getComponent<HealthComponent>().current_health = 100;

            m_objects.push_back(std::move(enemy));
        }
    }


    for (int i = 0; i < N * M + 1; i++) {

        std::unique_ptr<GameObject> bullet = std::make_unique<GameObject>();

        AnimationComponent animation_component;
        animation_component.animations[ static_cast<int>( EmittorState::PLAYER    ) ] = asset_manager.getAnimation("player_missile");
        animation_component.animations[ static_cast<int>( EmittorState::MISSILE   ) ] = asset_manager.getAnimation("missile");
        animation_component.animations[ static_cast<int>( EmittorState::INVADER_A ) ] = asset_manager.getAnimation("projectile_A");
        animation_component.animations[ static_cast<int>( EmittorState::INVADER_B ) ] = asset_manager.getAnimation("projectile_B");
        animation_component.animations[ static_cast<int>( EmittorState::INVADER_C ) ] = asset_manager.getAnimation("projectile_C");

        animation_component.current_state = static_cast<int>( EmittorState::PLAYER );
        animation_component.speed = 30.0f;


        ColliderComponent collider_component;
        collider_component.bounds.w = animation_component.animations[ static_cast<int>( EmittorState::PLAYER ) ]->frames[0].w;
        collider_component.bounds.h = animation_component.animations[ static_cast<int>( EmittorState::PLAYER ) ]->frames[0].h;
        collider_component.layer = CollisionLayer::INVADER_BULLET;
        collider_component.mask = CollisionLayer::PLAYER;
        bullet->addComponent<ColliderComponent>();
        bullet->addComponent<AnimationComponent>(std::move(animation_component));


        SoundComponent sound_component;
        sound_component.sounds[ static_cast<int>( SoundState::IDLE ) ] = "shoot"; //asset_manager.getSound("fastinvader1");
        sound_component.current_state = static_cast<int>( SoundState::IDLE );
        bullet->addComponent<SoundComponent>(std::move(sound_component));

        bullet->addComponent<TransformComponent>();
        bullet->getComponent<TransformComponent>().position = glm::vec2(400, 400);
        bullet->getComponent<TransformComponent>().scale = 2.0f;
        bullet->addComponent<SpriteComponent>();
        bullet->addComponent<PhysicsComponent>();
        bullet->addComponent<EmitterComponent>();

        bullet->addComponent<HealthComponent>();

        bullet->setActive(false);

        m_objects.push_back(std::move(bullet));
    }

    m_state = std::make_unique<RunState>(*this);
    m_state->onEnter();

    return true;
}


void Asteroids::handleInput(InputManager &input_manager) {

    std::unique_ptr<IGameState> new_state = m_state->handleInput(input_manager);

    if (new_state != nullptr) {

        m_state->onExit();
        m_state = std::move(new_state);
        m_state->onEnter();
    }
}


bool Asteroids::update(float deltatime, Engine& engine) {

    std::unique_ptr<IGameState> new_state = m_state->update(deltatime, engine);

    if (new_state != nullptr) {

        m_state->onExit();
        m_state = std::move(new_state);
        m_state->onEnter();
    }

    m_event_bus->clear();

    return true;
}

void Asteroids::render(RenderSystem& renderer) {

    m_state->render(renderer);
}




