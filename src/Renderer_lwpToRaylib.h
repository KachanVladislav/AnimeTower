#pragma once
#include <span>
#include "raylib.h"
#include "lwp/lwp.h"

struct Renderer_lwpToRaylib
{
    static void drawRectangles(std::span<const lwp::Rectangle> rectangles, Color color)
    {
        for (const auto &rect : rectangles)
        {
            DrawRectangleV({rect.bottomLeftPos.x, -rect.bottomLeftPos.y - rect.size.y}, {rect.size.x, rect.size.y}, color);
        }
    }
};