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
    static Rectangle lwpToRaylib_rectangle(const lwp::Rectangle &rect_lwp)
    {
        Vector2 pos = lwpToRaylib_position(rect_lwp.getPos_TopLeft());
        return Rectangle {pos.x, pos.y, rect_lwp.width, rect_lwp.height};
    }

    static void drawRectangles(std::span<const lwp::Rectangle> rectangles, Color color)
    {
        for (const auto &rect : rectangles)
        {
            Vector2 raylibPos = lwpToRaylib_position(rect.getPos_TopLeft());
            DrawRectangleV(raylibPos, {rect.width, rect.height}, color);
            DrawCircleV(raylibPos, 0.05f, BLACK);
        }
    }

    static void drawTexture_sameHigh(const lwp::Rectangle &rect, const Texture2D &texture, bool isFacingRight)
    {
        const Vector2 posTopLeft = lwpToRaylib_position(rect.getPos_TopLeft());
        const float textureSizeX = (float)texture.width * rect.height / (float)texture.height;
        const float texturePosX = posTopLeft.x - (textureSizeX - rect.width) / 2;

        Rectangle destRect = {texturePosX, posTopLeft.y, textureSizeX, rect.height};
        Rectangle sourceRect = {0, 0, (float)texture.width, (float)texture.height};
        Vector2 origin = {0, 0};

        if (!isFacingRight)
            sourceRect.width = -sourceRect.width;

        DrawTexturePro(texture, sourceRect, destRect, origin, 0.0f, WHITE);
    }
};
