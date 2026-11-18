#pragma once
#include "../atLevel/atLevelData.h"
#include "../atRender/atRenderLevel.h"
#include "../atConvert_lwpToRaylib.h"
#include "atLevelDesignerScreenLayout.h"

struct atLevelDesigner_Data
{
    Camera2D camera;
    atLevelData levelData;
    atLevelDesignerScreenLayout screenLayout;
};