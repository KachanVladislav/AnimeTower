#include "raylib.h"
#include "InputSystem.h"
#include "WidgetInputSystemTest.h"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InputSystem inputSystem;

    InitWindow(screenWidth, screenHeight, "AnimeTower");
    SetTargetFPS(120);

    Font font = LoadFontEx("./resource/GoogleSans-VariableFont_GRAD,opsz,wght.ttf", 64, NULL, 0);
    WidgetInputSystemTest widgetInput;
    widgetInput.settings = WidgetInputSystemTestSettings::buildDefault(Vector2{10.f, 30.f}, font);

    while (!WindowShouldClose())
    {
        InputSystemResult input = inputSystem.update();

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawFPS(10, 10);
        widgetInput.draw(input);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
