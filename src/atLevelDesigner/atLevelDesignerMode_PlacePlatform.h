#pragma once
#include "atLevelDesignerModeInterface.h"

struct atLevelDesignerMode_PlacePlatform : public atLevelDesignerModeInterface
{
    bool is1stCornerSelected = false;
    lwp::Vector2f firstCornerPos_lwp;

    void beforeRender(atLevelDesigner_Data &data) override
    {
        if (!is1stCornerSelected)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                is1stCornerSelected = true;
                firstCornerPos_lwp = getMousePos_lwp(data.camera);
            }
        }
        else
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                const lwp::Vector2f secondCornerPos_lwp = getMousePos_lwp(data.camera);
                lwp::Rectangle platform = buildRectangleByPos_lwp(firstCornerPos_lwp, secondCornerPos_lwp);
                if (platform.width > 0.01f && platform.height > 0.01f)
                {
                    is1stCornerSelected = false;
                    data.levelData.horizontalPlatforms_lwp.push_back(platform);
                    data.renderData.horizontalPlatforms_raylib = atConvert_lwpToRaylib::buildRectsVector(data.levelData.horizontalPlatforms_lwp);
                }
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                is1stCornerSelected = false;
            }
        }
    }
    void afterRender(atLevelDesigner_Data &) override
    {
    }

    void render_2D(atLevelDesigner_Data &data) override
    {
        if (is1stCornerSelected)
        {
            lwp::Vector2f mousePos_lwp = getMousePos_lwp(data.camera);
            lwp::Rectangle rect_lwp = buildRectangleByPos_lwp(mousePos_lwp, firstCornerPos_lwp);
            Rectangle rect_raylib = atConvert_lwpToRaylib::rectangle(rect_lwp);
            DrawRectangleLinesEx(rect_raylib, 2.0f, GREEN);
        }
    }

    void render_Screen(atLevelDesigner_Data &) override
    {
        if (is1stCornerSelected)
            StatusBar_renderText("build Platform | click 2nd corner");
        else
            StatusBar_renderText("build Platform | click 1st corner");
    }
    void onStart(atLevelDesigner_Data &) override {}
    void onExit(atLevelDesigner_Data &) override
    {
        is1stCornerSelected = false;
    }
};