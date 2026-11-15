#pragma once
#include "atLevelDesignerModeInterface.h"
#include "atLevelDesignerSelection.h"

struct atLevelDesignerMode_Hub : public atLevelDesignerModeInterface
{
    static inline const char *LEVEL_PATH_TO_SAVE = "./resource/levels/levelFromDesigner.txt";
    atLevelDesignerSelection selection;

    void beforeRender(atLevelDesigner_Data &data) override
    {
        selection.update(data);
    }
    void afterRender(atLevelDesigner_Data &) override
    {
    }
    void render_2D(atLevelDesigner_Data &data) override
    {
        selection.render(data);
    }
    void render_Screen(atLevelDesigner_Data &data) override
    {
        if (GuiButton({GetScreenWidth() - 80.f - 10, 10, 80, 30}, "Save"))
        {
            atLevelDataLoader::saveLevelSourceData(LEVEL_PATH_TO_SAVE, data.levelData);//should not be here
        }
        if (!selection.selectedPlatformIndices.empty())
        {
            std::string text = std::format("selected: {} platform(s) | D - delete", selection.selectedPlatformIndices.size());
            StatusBar_renderText(text.c_str());
        }
        else if (!selection.undoStack.empty())
        {
            StatusBar_renderText("designer | B - build, F1 - play | Ctrl+Z - undo");
        }
        else
        {
            StatusBar_renderText("designer | B - build, F1 - play");
        }
    }
    void onStart(atLevelDesigner_Data &) override
    {
    }
    void onExit(atLevelDesigner_Data &) override
    {
        selection.clear();
    }
};
