#pragma once
#include "atPlatformRender.h"

struct atPlatformRenderAtlasLoader
{
    // default one for now
    // 1 row, 3 columns, size {3*h, h}
    static atPlatformRender::Atlas load_1_3(const Texture2D &texture)
    {
        atPlatformRender::Atlas res;
        res.texture = texture;

        const float h = (float)texture.height;
        res.recLeft = {0, 0, h, h};
        res.recMid = {h, 0, h, h};
        res.recRight = {2 * h, 0, h, h};
        res.offset_y = -h / 4;
        return res;
    }
};