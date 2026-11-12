#pragma once
#include "raylib.h"

struct atObjectRender
{
    struct Atlas
    {
        Texture2D texture;
        Rectangle frames[32];
        int frameQty = 0;
    };

    static void drawFrame(const Atlas &atlas, int frameIndex, const Rectangle &destRect)
    {
        if (frameIndex < 0 || frameIndex >= atlas.frameQty)
            return;

        Vector2 origin = {0, 0};
        DrawTexturePro(atlas.texture, atlas.frames[frameIndex], destRect, origin, 0.0f, WHITE);
    }

    static void drawAnimated(const Atlas &atlas, float animationTime, float frameDuration, const Rectangle &destRect)
    {
        if (atlas.frameQty == 0)
            return;

        int frameIndex = (int)(animationTime / frameDuration) % atlas.frameQty;
        drawFrame(atlas, frameIndex, destRect);
    }
};
