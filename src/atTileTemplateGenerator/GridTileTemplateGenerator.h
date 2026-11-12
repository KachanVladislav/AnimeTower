#pragma once
#include <cstdio>
#include "raylib.h"

struct GridTileTemplateSettings
{
    int columns;
    int rows;
    int width;
    int height;
    int floorLevel;
};

struct GridTileTemplateSettingsBuilder
{
    static GridTileTemplateSettings platform()
    {
        GridTileTemplateSettings res;
        res.columns = 3;
        res.rows = 1;
        res.width = 64;
        res.height = 64;
        res.floorLevel = 48;
        return res;
    }
    static GridTileTemplateSettings grid(int columns, int rows, int width, int height)
    {
        GridTileTemplateSettings res;
        res.columns = columns;
        res.rows = rows;
        res.width = width;
        res.height = height;
        res.floorLevel = 0;
        return res;
    }
    static GridTileTemplateSettings grid_2_2_64_128()
    {
        GridTileTemplateSettings res;
        res.columns = 2;
        res.rows = 2;
        res.width = 64;
        res.height = 128;
        res.floorLevel = 0;
        return res;
    }
};

struct GridTileTemplateGenerator
{
    static inline const Color COLOR_GRID = {255, 0, 0, 128};
    static inline const Color COLOR_FLOOR = {0, 255, 0, 128};
    static void generate(const char *dirResults, const char *fileNameBase, const GridTileTemplateSettings &settings)
    {
        int imgWidth = settings.columns * settings.width;
        int imgHeight = settings.rows * settings.height;

        Image image = GenImageColor(imgWidth, imgHeight, BLANK);

        for (int col = 1; col < settings.columns; col++)
        {
            int x = col * settings.width;
            ImageDrawLine(&image, x, 0, x, imgHeight, COLOR_GRID);
        }

        for (int row = 0; row < settings.rows; row++)
        {
            int y = (1 + row) * settings.height;
            if (settings.floorLevel != 0)
                ImageDrawLine(&image, 0, y - settings.floorLevel, imgWidth, y - settings.floorLevel, COLOR_FLOOR);
            ImageDrawLine(&image, 0, y, imgWidth, y, COLOR_GRID);
        }

        char filename[256];
        sprintf_s(filename, "%s%s_%d_%d_%d.png", dirResults, fileNameBase, settings.rows, settings.columns, settings.width);
        ExportImage(image, filename);
        UnloadImage(image);
    }
};