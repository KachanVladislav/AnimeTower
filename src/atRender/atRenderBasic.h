#pragma once
#include <raylib.h>
#include <span>

struct atRenderBasic
{
    static void drawRectangles(std::span<Rectangle> rectangles, Color color)
    {
        for (const auto &rect : rectangles)
        {
            DrawRectangleV({rect.x, rect.y}, {rect.width, rect.height}, color);
            DrawCircleV({rect.x, rect.y}, 0.05f, BLACK);
        }
    }
};