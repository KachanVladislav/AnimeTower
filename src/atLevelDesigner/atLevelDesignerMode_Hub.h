#pragma once
#include "atLevelDesignerMode_Interface.h"
#include "atLevelDesignerSelection.h"
#include "hud/atWidgetObjectInspector.h"
#include "hud/atWidgetLayersOverview.h"

struct atLevelDesignerMode_Hub : public atLevelDesignerModeInterface
{
    static inline const char *LEVEL_PATH_TO_SAVE = "./resource/levels/levelFromDesigner.txt";
    atLevelDesignerSelection selection;
    atWidgetObjectInspector widgetObjectInspector;
    atWidgetLayersOverview widgetLayersOverview;
    bool inspectorInitialized = false;

    void beforeRender(atLevelDesigner_Data &data, atLevelDesignerOp_Executer &opExecuter) override
    {
        data.screenLayout.update((float)GetScreenWidth(), (float)GetScreenHeight());
        selection.update(data);
        if (IsKeyPressed(KEY_D))
        {
            deleteSelected(data, opExecuter);
        }
    }
    void afterRender(atLevelDesigner_Data &, atLevelDesignerOp_Executer &) override
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
            atLevelDataLoader::saveLevelSourceData(LEVEL_PATH_TO_SAVE, data.levelData); // should not be here
        }
        if (!selection.selectedObjectsRef.empty())
        {
            std::string text = std::format("selected: {} platform(s) | D - delete", selection.selectedObjectsRef.size());
            StatusBar_renderText(text.c_str());
        }
        else
            StatusBar_renderText("designer | B - build, F1 - play");

        if (!inspectorInitialized)
        {
            Font font = LoadFontEx("./resource/GoogleSans-VariableFont_GRAD,opsz,wght.ttf", 64, NULL, 0);
            widgetObjectInspector.theme = atWidgetTheme::buildDefault(font);
            widgetLayersOverview.theme = atWidgetTheme::buildDefault(font);
            inspectorInitialized = true;
        }
        widgetLayersOverview.draw(data.levelData, data.screenLayout.scene);
        widgetObjectInspector.draw(selection.selectedObjectsRef, data.screenLayout.inspector, data.levelData);
    }
    void onStart(atLevelDesigner_Data &) override
    {
    }
    void onExit(atLevelDesigner_Data &) override
    {
        selection.clear();
    }

private:
    void deleteSelected(atLevelDesigner_Data &data, atLevelDesignerOp_Executer &opExecuter)
    {
        if (selection.selectedObjectsRef.empty())
            return;

        auto op = std::make_unique<atLevelDesignerOp_DeleteObjects>(selection.selectedObjectsRef);
        opExecuter.execute(std::move(op), data);
        selection.clear();
    }
};
