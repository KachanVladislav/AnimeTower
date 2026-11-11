#pragma once
#include <span>
#include "raylib.h"
#include "lwp/lwp.h"

struct Renderer_lwpToRaylib
{
    static Vector2 lwpToRaylib_position(const lwp::Vector2f &pos_lwp)
    {
        return Vector2{pos_lwp.x, -pos_lwp.y};
    }

    static void drawRectangles(std::span<const lwp::Rectangle> rectangles, Color color)
    {
        for (const auto &rect : rectangles)
        {
            Vector2 raylibPos = lwpToRaylib_position(rect.getPos_TopLeft());
            DrawRectangleV(raylibPos, {rect.size.x, rect.size.y}, color);
            DrawCircleV(raylibPos, 0.05f, BLACK);
        }
    }

    static void drawTexture_sameHigh(const lwp::Rectangle &rect, const Texture2D &texture, bool isDirectionRight)
    {
        const Vector2 posTopLeft = lwpToRaylib_position(rect.getPos_TopLeft());
        const float textureSizeX = (float)texture.width * rect.size.y / (float)texture.height;
        const float texturePosX = posTopLeft.x - (textureSizeX - rect.size.x) / 2;

        Rectangle destRect = {texturePosX, posTopLeft.y, textureSizeX, rect.size.y};
        Rectangle sourceRect = {0, 0, (float)texture.width, (float)texture.height};
        Vector2 origin = {0, 0};

        if (!isDirectionRight)
            sourceRect.width = -sourceRect.width;

        DrawTexturePro(texture, sourceRect, destRect, origin, 0.0f, WHITE);
    }
};
