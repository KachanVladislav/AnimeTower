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

        //draw render data
        for (auto &renderData : data.layersRenderData)
        {
            Vector2 offset{offsetBase.x * offsetCount, offsetBase.y * offsetCount};
            offsetCount++;
            for (auto &sprite : renderData.sprites.vec)
            {
                lwp::Vector2f pos = sprite.objRef.resolve()->transform.pos;
                Vector2 pos_raylib{pos.x + sprite.offset.x, -pos.y - sprite.offset.y - sprite.size.y};
                auto rect = Rectangle{pos_raylib.x + offset.x, pos_raylib.y + offset.y, sprite.size.x, sprite.size.y};
                drawTexture_sameHigh(rect, textures.playerTexture, sprite.isFlipX);
            }
            for (auto &sprite : renderData.platforms.vec)
            {
                lwp::Vector2f pos = sprite.objRef.resolve()->transform.pos;
                Vector2 pos_raylib{pos.x + sprite.offset.x, -pos.y - sprite.offset.y - sprite.size.y};
                auto rect = Rectangle{pos_raylib.x + offset.x, pos_raylib.y + offset.y, sprite.size.x, sprite.size.y};
                atRenderPlatform::draw(textures.platformAtlas, rect);
            }
            for (auto &sprite : renderData.animated.vec)
            {
                lwp::Vector2f pos = sprite.objRef.resolve()->transform.pos;
                Vector2 pos_raylib{pos.x + sprite.offset.x, -pos.y - sprite.offset.y - sprite.size.y};
                auto rect = Rectangle{pos_raylib.x + offset.x, pos_raylib.y + offset.y, sprite.size.x, sprite.size.y};
                atObjectRender::drawAnimated(textures.flagPolAtlas, (float)GetTime(), 0.15f, rect);
            }
        }

        //draw colliders
        offsetCount = -(int)data.currentLayerNum;
        for (auto &layer : data.layers)
        {
            Vector2 offset{offsetBase.x * offsetCount, offsetBase.y * offsetCount};
            offsetCount++;
            for (const auto &obj : layer.colStatic.vec)
            {
                lwp::Vector2f pos = obj.transform.pos;
                atColliderRect collider = obj.collider;
                Vector2 pos_raylib{pos.x + collider.offset.x, -pos.y - collider.offset.y - collider.size.y};
                auto rect = Rectangle{pos_raylib.x + offset.x, pos_raylib.y + offset.y, collider.size.x, collider.size.y};
                DrawRectangleLinesEx(rect, 0.05f, BLUE);
                DrawCircleV({rect.x, rect.y}, 0.05f, BLACK);
            }

            for (const auto &obj : layer.colRigid.vec)
            {
                lwp::Vector2f pos = obj.transform.pos;
                atColliderRect collider = obj.collider;
                Vector2 pos_raylib{pos.x + collider.offset.x, -pos.y - collider.offset.y - collider.size.y};
                auto rect = Rectangle{pos_raylib.x + offset.x, pos_raylib.y + offset.y, collider.size.x, collider.size.y};
                DrawRectangleLinesEx(rect, 0.05f, GREEN);
                DrawCircleV({rect.x, rect.y}, 0.05f, BLACK);
            }

            for (const auto &obj : layer.colNotCollidable.vec)
            {
                lwp::Vector2f pos = obj.transform.pos;
                atColliderRect collider = obj.collider;
                Vector2 pos_raylib{pos.x + collider.offset.x, -pos.y - collider.offset.y - collider.size.y};
                auto rect = Rectangle{pos_raylib.x + offset.x, pos_raylib.y + offset.y, collider.size.x, collider.size.y};
                DrawRectangleLinesEx(rect, 0.05f, YELLOW);
                DrawCircleV({rect.x, rect.y}, 0.05f, BLACK);
            }
        }
    }
};
