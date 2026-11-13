#pragma once
#include "raylib.h"
#include "../atResource/atResourceAnimationAtlas.h"

struct atObjectRender
{
    static void drawFrame(const atResourceAnimationAtlas &atlas, int frameIndex, const Rectangle &destRect)
    {
        if (frameIndex < 0 || frameIndex >= atlas.frameQty)
            return;

        Vector2 origin = {0, 0};
        DrawTexturePro(atlas.texture, atlas.frames[frameIndex], destRect, origin, 0.0f, WHITE);
    }

    static void drawAnimated(const atResourceAnimationAtlas &atlas, float animationTime, float frameDuration, const Rectangle &destRect)
    {
        if (atlas.frameQty == 0)
            return;

        int frameIndex = (int)(animationTime / frameDuration) % atlas.frameQty;
        drawFrame(atlas, frameIndex, destRect);
    }
};
