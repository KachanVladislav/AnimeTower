#pragma once
#include "atLevelDesignerMode_Interface.h"
#include "atLevelDesignerSelection.h"
#include "../hud/WidgetObjectInspector.h"

struct atLevelDesignerMode_Hub : public atLevelDesignerModeInterface
{
    static inline const char *LEVEL_PATH_TO_SAVE = "./resource/levels/levelFromDesigner.txt";
    atLevelDesignerSelection selection;
    WidgetObjectInspector objectInspector;
    bool inspectorInitialized = false;

    void beforeRender(atLevelDesigner_Data &data, atLevelDesignerOp_Executer &opExecuter) override
    {
        data.screenLayout.update((float)GetScreenWidth(), (float)GetScreenHeight());
        selection.update(data);
        if (IsKeyPressed(KEY_DELETE))
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
        if (!selection.selectedPlatformIds.empty())
        {
            std::string text = std::format("selected: {} platform(s) | Delete - delete", selection.selectedPlatformIds.size());
            StatusBar_renderText(text.c_str());
        }
        else
            StatusBar_renderText("designer | B - build, F1 - play");

        if (!inspectorInitialized)
        {
            Font font = LoadFontEx("./resource/GoogleSans-VariableFont_GRAD,opsz,wght.ttf", 64, NULL, 0);
            objectInspector.settings = WidgetObjectInspectorSettings::buildDefault(font);
            inspectorInitialized = true;
        }
        objectInspector.draw(selection.selectedPlatformIds, data.levelData.objectsStatic, data.screenLayout.inspector);
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
        if (selection.selectedPlatformIds.empty())
            return;
        std::vector<atObject> platforms;
        std::vector<uint32_t> ids = selection.selectedPlatformIds;
        for (uint32_t id : ids)
        {
            for (const auto &obj : data.levelData.objectsStatic)
            {
                if (obj.id == id)
                {
                    platforms.push_back(obj);
                    break;
                }
            }
        }
        auto op = std::make_unique<atLevelDesignerOp_DeletePlatforms>(std::move(platforms), std::move(ids));
        opExecuter.execute(std::move(op), data);
        selection.clear();
    }
};
