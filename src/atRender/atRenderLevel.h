#pragma once
#include <raylib.h>
#include "../atLevel/atLevelData.h"
#include "../atResource/atResourceRender.h"
#include "atRenderPlatform.h"
#include "atRenderAnimation.h"

struct atRenderLevelData
{
    std::vector<Rectangle> horizontalPlatforms_raylib;
    std::vector<Rectangle> objectsRects_raylib;

    Rectangle playerRect;
    bool isPlayerFaceRight = true;
};

static inline void drawTexture_sameHigh(const Rectangle &rect, const Texture2D &texture, bool isFacingRight)
{
    const float textureSizeX = (float)texture.width * rect.height / (float)texture.height;
    const float texturePosX = rect.x - (textureSizeX - rect.width) / 2;

    Rectangle destRect = {texturePosX, rect.y, textureSizeX, rect.height};
    Rectangle sourceRect = {0, 0, (float)texture.width, (float)texture.height};
    Vector2 origin = {0, 0};

    if (!isFacingRight)
        sourceRect.width = -sourceRect.width;

    DrawTexturePro(texture, sourceRect, destRect, origin, 0.0f, WHITE);
}

struct atRenderLevel
{
    static void render(const atRenderLevelData &renderData, const atResourceRender &textures)
    {
        ClearBackground(LIGHTGRAY);
        atRenderPlatform::drawSpan(textures.platformAtlas, renderData.horizontalPlatforms_raylib);
        // Renderer_lwpToRaylib::drawRectangles({&player.box, 1}, PINK);
        drawTexture_sameHigh(renderData.playerRect, textures.playerTexture, renderData.isPlayerFaceRight);
    
        for (const auto &object : renderData.objectsRects_raylib)
            atObjectRender::drawAnimated(textures.flagPolAtlas, (float)GetTime(), 0.15f, object);
    }
};
