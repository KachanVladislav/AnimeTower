#include "raylib.h"
#include "InputSystem.h"
#include "Console.h"
#include "atPhysics.h"
#include "atCamera.h"
#include "hud/hud.h"
#include "Renderer_lwpToRaylib.h"
#include "atPlatformBuilder.h"
#include "atTextureLoader.h"

struct atTextures
{
    Texture2D playerTexture;
    void load(atTextureLoader &textureLoader)
    {
        playerTexture = textureLoader.loadTextureFromFile("./resource/elBuba.png");
    }
    void unload()
    {
        UnloadTexture(playerTexture);
    }
};

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    const float screenWidth_lwp = 20;
    SetTraceLogLevel(LOG_WARNING); // raylib  logs

    atPlatformBuilder platformBuilder;

    lwp::Rectangle staticBoxes[] = {
        // lwp::Rectangle::build_BottomCenter({100.f, 300.f}, {50.f, 100.f}),
        // lwp::Rectangle::build_BottomCenter({500.f, 300.f}, {50.f, 100.f}),
        platformBuilder.buildHorizontalLeft({0.f, 2.7f}, 10.f),
        platformBuilder.buildHorizontalRight({5.f, 2.7f}, 10.f),
        platformBuilder.buildHorizontalRight({7.f, 5.4f}, 3.f),
        platformBuilder.buildHorizontalRight({-2.f, 1.5f}, 20.f)};

    lwp::RigidBody2D player;
    lwp::Vector2f playerSize{1.f, 2.f};
    player.box = lwp::Rectangle::build_BottomCenter({1.1f, 2.f}, playerSize);

    InitWindow(screenWidth, screenHeight, "AnimeTower");
    SetTargetFPS(120);

    InputSystem inputSystem;

    atTextureLoader textureLoader;
    textureLoader.load();
    atTextures textures;
    textures.load(textureLoader);

    atCamera camera;
    camera.initialize(screenWidth, screenHeight, screenWidth_lwp);

    HUD hud;
    hud.initialize();

    bool currentDirectionToRight = true;
    while (!WindowShouldClose())
    {
        float dT = GetFrameTime(); // if less than cap than cap?
        InputSystemResult input = inputSystem.update();
        atPhysics::applyInputToRigidBody(player, input);
        atPhysics::updatePhysics(player, staticBoxes, dT);

        BeginDrawing();

        camera.setTarget_lwp(player.box.getCenter());
        BeginMode2D(camera.camera);
        ClearBackground(LIGHTGRAY);
        Renderer_lwpToRaylib::drawRectangles(staticBoxes, GREEN);
        // Renderer_lwpToRaylib::drawRectangles({&player.box, 1}, PINK);
        if (input.direction)
            currentDirectionToRight = (*input.direction == InputSystemDirection::right);
        Renderer_lwpToRaylib::drawTexture_sameHigh(player.box, textures.playerTexture, currentDirectionToRight);
        EndMode2D();

        hud.draw(input);

        EndDrawing();
    }

    textures.unload();
    textureLoader.unload();
    CloseWindow();

    return 0;
}
