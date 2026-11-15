#pragma once
#include "../atLevel/atLevelData.h"
#include "../atRender/atRenderLevel.h"
#include "../atConvert_lwpToRaylib.h"

struct atLevelDesigner_Data
{
    Camera2D camera;
    atLevelData levelData;
    lwp::RigidBody2D player;
    atRenderLevelData renderData;

    void updateRenderData()
    {
        renderData.horizontalPlatforms_raylib = atConvert_lwpToRaylib::buildRectsVector(levelData.horizontalPlatforms_lwp);
        renderData.objectsRects_raylib = atConvert_lwpToRaylib::buildRectsVector(levelData.objectsRects_lwp);
        renderData.playerRect = atConvert_lwpToRaylib::rectangle(player.rect);
    }
};