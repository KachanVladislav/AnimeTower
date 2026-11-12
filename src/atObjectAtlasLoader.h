#pragma once
#include "atObjectRender.h"

struct atObjectAtlasLoader
{
    static atObjectRender::Atlas loadGrid(const Texture2D &texture, int rows, int cols)
    {
        atObjectRender::Atlas res;
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
