#pragma once
#include "raylib.h"

struct atResourcePlatformAtlas
{
    Texture2D texture;
    Rectangle recLeft;
    Rectangle recMid;
    Rectangle recRight;
    float offset_y = 0;
};

struct atResourcePlatformAtlasLoader
{
    // default one for now
    // 1 row, 3 columns, size {3*h, h}
    static atResourcePlatformAtlas load_1_3(const Texture2D &texture)
    {
        atResourcePlatformAtlas res;
        res.texture = texture;

        const float h = (float)texture.height;
        res.recLeft = {0, 0, h, h};
        res.recMid = {h, 0, h, h};
        res.recRight = {2 * h, 0, h, h};
        res.offset_y = -h / 4;
        return res;
    }
};