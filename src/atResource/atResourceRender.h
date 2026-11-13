#pragma once
#include "atResourcePlatformAtlas.h"
#include  "atResourceAnimationAtlas.h"

struct atResourceRender
{
    atResourcePlatformAtlas platformAtlas;
    atResourceAnimationAtlas flagPolAtlas;
    Texture2D playerTexture;
    void load()
    {
        platformAtlas = atResourcePlatformAtlasLoader::load_1_3(loadTextureFromFile("./resource/tile_1_3_64.png"));
        flagPolAtlas = atResourceAnimationAtlasLoader::loadGrid(loadTextureFromFile("./resource/flagPoll_2_2_64_128.png"), 2, 2);
        playerTexture = loadTextureFromFile("./resource/elBuba.png");
    }
    void unload()
    {
        UnloadTexture(playerTexture);
        UnloadTexture(platformAtlas.texture);
        UnloadTexture(flagPolAtlas.texture);
    }
private:
    static Image loadImageFromFile(const char *fileName)
    {
        Image image = LoadImage(fileName);
        if (!IsImageValid(image))
        {
            TraceLog(LOG_ERROR, "Failed to load image: %s", fileName);
            return GenImageColor(8, 8, RED);
        }
        TraceLog(LOG_INFO, "Image loaded: %dx%d, format=%d", image.width, image.height, image.format);
        return image;
    }
    static Texture2D loadTextureFromImage(const Image &image, const char *errorText)
    {
        Texture2D texture = LoadTextureFromImage(image);
        if (!IsTextureValid(texture))
            TraceLog(LOG_ERROR, "Failed to create texture from image, %s", errorText);
        return texture;
    }
    static Texture2D loadTextureFromFile(const char *fileName)
    {
        Image image = loadImageFromFile(fileName);
        TraceLog(LOG_INFO, "Image {%s}: %dx%d, format=%d", fileName, image.width, image.height, image.format);
        ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
        Texture2D texture = loadTextureFromImage(image, fileName);
        UnloadImage(image);
        return texture;
    }
};