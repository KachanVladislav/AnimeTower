#pragma once
#include <raylib.h>
#include "../atLevel/atLevelData.h"
#include "../atResource/atResourceRender.h"
#include "atRenderPlatform.h"
#include "atRenderAnimation.h"
#include "atConvert_lwpToRaylib.h"

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
    static void render(atLevelData &data, const atResourceRender &textures)
    {
        ClearBackground(LIGHTGRAY);

        int offsetCount = -(int)data.currentLayerNum;
        Vector2 offsetBase{5.f, -5.f};
        for (auto &layer : data.layers)
        {
            Vector2 offset{offsetBase.x * offsetCount, offsetBase.y * offsetCount};
            offsetCount++;
            for (const auto &obj : layer.colStatic.vec)
            {
                Vector2 pos_raylib = atConvert_lwpToRaylib::position(obj.getPos_UpLeft());
                auto rect = Rectangle{pos_raylib.x + offset.x, pos_raylib.y + offset.y, obj.collider.size.x, obj.collider.size.y};
                atRenderPlatform::draw(textures.platformAtlas, rect);
                DrawCircleV({rect.x, rect.y}, 0.05f, BLACK);
                DrawLineEx({rect.x, rect.y}, {pos_raylib.x, pos_raylib.y}, 0.1f, BLACK);
                DrawLineEx({rect.x + rect.width, rect.y}, {pos_raylib.x + rect.width, pos_raylib.y}, 0.1f, BLACK);
            }

            for (const auto &obj : layer.colRigid.vec)
            {
                Vector2 pos_raylib = atConvert_lwpToRaylib::position(obj.getPos_UpLeft());
                auto rect = Rectangle{pos_raylib.x + +offset.x, pos_raylib.y + +offset.y, obj.collider.size.x, obj.collider.size.y};
                drawTexture_sameHigh(rect, textures.playerTexture, obj.isFlipX);
                DrawCircleV({rect.x, rect.y}, 0.05f, BLACK);
            }

            for (const auto &obj : layer.colNotCollidable.vec)
            {
                Vector2 pos_raylib = atConvert_lwpToRaylib::position(obj.getPos_UpLeft());
                auto rect = Rectangle{pos_raylib.x + +offset.x, pos_raylib.y + +offset.y, obj.collider.size.x, obj.collider.size.y};
                atObjectRender::drawAnimated(textures.flagPolAtlas, (float)GetTime(), 0.15f, rect);
                DrawCircleV({rect.x, rect.y}, 0.05f, BLACK);
            }
        }
    }
};
