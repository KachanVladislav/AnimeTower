#include "raylib.h"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylibStart - Basic Window");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello raylib!", 190, 200, 20, RED);
        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
