#include "raylib.h"
#include "InputSystem.h"
#include "Console.h"
#include "atPhysics.h"
#include "atCamera.h"
#include "hud/hud.h"
#include "Renderer_lwpToRaylib.h"
#include "atPlatformBuilder.h"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    const float screenWidth_lwp = 20;
    SetTraceLogLevel(LOG_WARNING);//raylib  logs
    
    InputSystem inputSystem;
    HUD hud;
    atCamera camera;
    camera.initialize(screenWidth, screenHeight, screenWidth_lwp);

    atPlatformBuilder platformBuilder;

    lwp::Rectangle staticBoxes[] = {
        // lwp::Rectangle::build_BottomCenter({100.f, 300.f}, {50.f, 100.f}),
        // lwp::Rectangle::build_BottomCenter({500.f, 300.f}, {50.f, 100.f}),
        platformBuilder.buildHorizontalLeft({0.f, 2.7f}, 10.f),
        platformBuilder.buildHorizontalRight({5.f, 2.7f}, 10.f),
        platformBuilder.buildHorizontalRight({7.f, 5.4f}, 3.f),
        platformBuilder.buildHorizontalRight({-2.f, 1.5f}, 20.f)};

    lwp::RigidBody2D player;
    player.box = lwp::Rectangle::build_BottomCenter({1.1f, 2.f}, {1.f, 2.f});

    InitWindow(screenWidth, screenHeight, "AnimeTower");
    hud.initialize();
    SetTargetFPS(120);

    while (!WindowShouldClose())
    {
        float dT = GetFrameTime(); // if less than cap than cap?
        InputSystemResult input = inputSystem.update();
        atPhysics::applyInputToRigidBody(player, input);
        atPhysics::updatePhysics(player, staticBoxes, dT);
        // Console::logRectangle("", player.box);

        camera.setTarget_lwp(player.box.getCenter());

        BeginDrawing();

        BeginMode2D(camera.camera);
        ClearBackground(RAYWHITE);
        Renderer_lwpToRaylib::drawRectangles(staticBoxes, GREEN);
        Renderer_lwpToRaylib::drawRectangles({&player.box, 1}, PINK);
        EndMode2D();

        hud.draw(input);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
