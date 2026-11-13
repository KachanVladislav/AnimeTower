#pragma once
#include <span>
#include "raylib.h"
#include "../atResource/atResourcePlatformAtlas.h"

struct atRenderPlatform
{
    static void draw(const atResourcePlatformAtlas &atlas, const Rectangle &destRect)
    {
        const float ratio_toDest = destRect.height / atlas.recLeft.height;

        const float widthDestLeft = atlas.recLeft.width * ratio_toDest;
        const float widthDestMid = atlas.recMid.width * ratio_toDest;
        const float widthDestRight = atlas.recRight.width * ratio_toDest;

        const float midXStart = destRect.x + widthDestLeft;
        const float rightXStart = destRect.x + destRect.width - widthDestRight;

        Vector2 origin = {0, 0};
        Rectangle destRectDraw = destRect;
        destRectDraw.y += atlas.offset_y * ratio_toDest;

        destRectDraw.width = widthDestLeft;
        DrawTexturePro(atlas.texture, atlas.recLeft, destRectDraw, origin, 0.0f, WHITE);

        destRectDraw.width = widthDestMid;
        for (float midX = midXStart; midX < rightXStart; midX += widthDestMid)
        {
            destRectDraw.x = midX;
            DrawTexturePro(atlas.texture, atlas.recMid, destRectDraw, origin, 0.0f, WHITE);
        }

        destRectDraw.x = rightXStart;
        destRectDraw.width = widthDestRight;
        DrawTexturePro(atlas.texture, atlas.recRight, destRectDraw, origin, 0.0f, WHITE);
    }

    static void drawSpan(const atResourcePlatformAtlas &atlas, std::span<const Rectangle> destRects)
    {
        for (const auto &rect : destRects)
        {
            atRenderPlatform::draw(atlas, rect);
            DrawCircleV({rect.x, rect.y}, 0.05f, BLACK);
        }
    }
};
