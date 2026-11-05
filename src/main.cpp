#include "raylib.h"
#include "InputSystem.h"
#include "WidgetInputSystemTest.h"

#include "lwp/lwp.h"
#include <span>

struct PlatformBuilderSettings
{
    float platformWidth = 0;
    float screenWidth = 0;
    static PlatformBuilderSettings buildDefault(float screenWidth)
    {
        PlatformBuilderSettings res;
        res.platformWidth = 30.f;
        res.screenWidth = screenWidth;
        return res;
    }
};

struct PlatformBuilder
{
    PlatformBuilderSettings settings;
    lwp::Rectangle buildHorizontal(lwp::Vector2f pos, float width)
    {
        return lwp::Rectangle::build_UpLeft(pos, {pos.x + width, settings.platformWidth});
    }
};

struct AnimeTowerPhysics
{
    static void updateBodySpeedToInput(lwp::RigidBody &body, const InputSystemResult &input)
    {
        const float MAX_SPEED = 20;
        const float JUMP_SPEED = 50;

        if (body.isOnFloor())
        {
            if (input.direction)
            {
                if (*input.direction == InputSystemDirection::right)
                    body.speed.x = MAX_SPEED;
                else
                    body.speed.x = -MAX_SPEED;
            }
            else
            {
                body.speed.x = 0;
            }
            if (input.action)
            {
                if (*input.action == InputSystemAction::jump)
                    body.speed.y = JUMP_SPEED;
            }
        }
    }

    static void updatePhysics(lwp::RigidBody &player, std::span<lwp::Rectangle> staticObjects, float dT)
    {
        const float g = 10.f;

        float playerBottomBefore = player.box.getY_Bottom();

        player.speed.y = player.speed.y - g * dT;
        player.applySpeedToBoxPos(dT);
        player.onFloor = false;

        float playerBottom = player.box.getY_Bottom();
        for (const auto box : staticObjects)
        {
            if (playerBottomBefore >= box.getY_Top() && playerBottom <= box.getY_Top()) // y-level
            {
                bool notAligned_x = player.box.getX_Right() < box.getX_Left() || player.box.getX_Left() > box.getX_Right();
                if (!notAligned_x) // x-level
                    player.setOnFloor(box.getY_Top());
            }
        }
    }

};

void drawBoxes(std::span<const lwp::Rectangle> objects, Color color)
{
    for (const auto &box : objects)
    {
        DrawRectangleV({box.bottomLeftPos.x, -box.bottomLeftPos.y - box.size.y}, {box.size.x, box.size.y}, color);
    }
}

#include <print>
class Console
{
public:
    template <typename... Args>
    static void log(std::format_string<Args...> fmt, Args &&...args)
    {
        std::print(fmt, std::forward<Args>(args)...);
    }

    static void logBox(const lwp::Rectangle &box)
    {
        log("x {:.1f} y {:.1f} bottom {:.1f}\n", box.bottomLeftPos.x, box.bottomLeftPos.y, box.getY_Bottom());
    }
};

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InputSystem inputSystem;

    Camera2D camera = {0};
    camera.target = Vector2{0, 0};
    camera.offset = Vector2{screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    PlatformBuilder platformBuilder;
    platformBuilder.settings = PlatformBuilderSettings::buildDefault((float)screenWidth);

    const float PLATFORM_WIDTH = 30.f;
    lwp::Rectangle staticBoxes[] = {
        lwp::Rectangle::build_BottomCenter({100.f, 300.f}, {50.f, 100.f}),
        lwp::Rectangle::build_BottomCenter({500.f, 300.f}, {50.f, 100.f}),
        platformBuilder.buildHorizontal({50.f, 50.f}, 500.f),
        platformBuilder.buildHorizontal({320.f, 150.f}, 200.f)};

    lwp::RigidBody player;
    player.box = lwp::Rectangle::build_BottomCenter({100.f, screenHeight - 2 * PLATFORM_WIDTH}, {20.f, 40.f});

    InitWindow(screenWidth, screenHeight, "AnimeTower");
    SetTargetFPS(120);

    Font font = LoadFontEx("./resource/GoogleSans-VariableFont_GRAD,opsz,wght.ttf", 64, NULL, 0);
    WidgetInputSystemTest widgetInput;
    widgetInput.settings = WidgetInputSystemTestSettings::buildDefault(Vector2{10.f, 30.f}, font);

    while (!WindowShouldClose())
    {
        InputSystemResult input = inputSystem.update();
        AnimeTowerPhysics::updateBodySpeedToInput(player, input);
        AnimeTowerPhysics::updatePhysics(player, staticBoxes, 0.1f); // todo add dT
        Console::logBox(player.box);

        camera.target = Vector2{player.box.bottomLeftPos.x, -player.box.bottomLeftPos.y};
        BeginDrawing();

        BeginMode2D(camera);
        ClearBackground(RAYWHITE);
        drawBoxes(staticBoxes, GREEN);
        drawBoxes({&player.box, 1}, PINK);
        EndMode2D();

        DrawFPS(10, 10);
        widgetInput.draw(input);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
