#include "raylib.h"
#include "InputSystem.h"
#include "Console.h"
#include "atPhysics.h"
#include "atCamera.h"
#include "hud/hud.h"

struct PlatformBuilder
{
    static inline const float PLATFORM_WIDTH = 0.5f;
    lwp::Rectangle buildHorizontalRight(lwp::Vector2f pos, float width)
    {
        return lwp::Rectangle::build_UpLeft(pos, {width, PLATFORM_WIDTH});
    }
    lwp::Rectangle buildHorizontalLeft(lwp::Vector2f pos, float width)
    {
        return buildHorizontalRight(lwp::Vector2f{pos.x - width, pos.y}, width);
    }
};

void drawRectangles(std::span<const lwp::Rectangle> objects, Color color)
{
    for (const auto &box : objects)
    {
        DrawRectangleV({box.bottomLeftPos.x, -box.bottomLeftPos.y - box.size.y}, {box.size.x, box.size.y}, color);
    }
}

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    const float screenWidth_lwp = 20;

    InputSystem inputSystem;
    HUD hud;
    atCamera camera;
    camera.initialize(screenWidth, screenHeight, screenWidth_lwp);

    PlatformBuilder platformBuilder;

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
        float dT = GetFrameTime(); //if less than cap than cap?
        InputSystemResult input = inputSystem.update();
        atPhysics::applyInputToRigidBody(player, input);
        atPhysics::updatePhysics(player, staticBoxes, dT);
        // Console::logRectangle("", player.box);

        camera.setTarget_lwp(player.box.getCenter());

        BeginDrawing();

        BeginMode2D(camera.camera);
        ClearBackground(RAYWHITE);
        drawRectangles(staticBoxes, GREEN);
        drawRectangles({&player.box, 1}, PINK);
        EndMode2D();

        hud.draw(input);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
