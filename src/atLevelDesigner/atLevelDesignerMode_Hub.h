#pragma once
#include "atLevelDesignerModeInterface.h"

struct atLevelDesignerMode_Hub : public atLevelDesignerModeInterface
{
    static inline const char *LEVEL_PATH_TO_SAVE = "./resource/levels/levelFromDesigner.txt";
    void beforeRender(atLevelDesigner_Data &) override
    {
    }
    void afterRender(atLevelDesigner_Data &) override
    {
    }
    void render_2D(atLevelDesigner_Data &) override
    {
    }
    void render_Screen(atLevelDesigner_Data &data) override
    {
        
        if (GuiButton({GetScreenWidth() - 80.f - 10, 10, 80, 30}, "Save"))
        {
            atLevelDataLoader::saveLevelSourceData(LEVEL_PATH_TO_SAVE, data.levelData);//should not be here
        }
        StatusBar_renderText("designer | B - build, F1 - play");
    }
    void onStart(atLevelDesigner_Data &) override
    {
    }
    void onExit(atLevelDesigner_Data &) override
    {
    }
};