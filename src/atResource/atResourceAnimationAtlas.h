#pragma once
#include "raylib.h"

struct atResourceAnimationAtlas
{
    Texture2D texture;
    Rectangle frames[32];
    int frameQty = 0;
};

struct atResourceAnimationAtlasLoader
{
    static atResourceAnimationAtlas loadGrid(const Texture2D &texture, int rows, int cols)
    {
        atResourceAnimationAtlas res;
        res.texture = texture;
        res.frameQty = rows * cols;

        const float frameW = (float)texture.width / cols;
        const float frameH = (float)texture.height / rows;

        for (int i = 0; i < res.frameQty; i++)
        {
            int col = i % cols;
            int row = i / cols;
            res.frames[i] = {col * frameW, row * frameH, frameW, frameH};
        }
        return res;
    }
};
