#pragma once
#include "atLevelDesignerMode_Interface.h"
#include "../InputSystem.h"
#include "../atPhysics.h"
#include "../atDefaults.h"

struct atLevelDesignerMode_PlayTest : public atLevelDesignerModeInterface
{
    InputSystem inputSystem;
    void beforeRender(atLevelDesigner_Data &data, atLevelDesignerOp_Executer&) override
    {
        InputSystemResult input = inputSystem.update();
        if (input.direction)
            data.renderData.isPlayerFaceRight = (*input.direction == InputSystemDirection::right);
        float dT = GetFrameTime();
        atPhysics::applyInputToRigidBody(data.player, input);
        atPhysics::updatePhysics(data.player, data.levelData.horizontalPlatforms_lwp, dT);

        data.renderData.playerRect = atConvert_lwpToRaylib::rectangle(data.player.rect);
    }
    void afterRender(atLevelDesigner_Data &, atLevelDesignerOp_Executer&) override
    {
    }
    void render_2D(atLevelDesigner_Data &) override
    {
    }
    void render_Screen(atLevelDesigner_Data &) override
    {
        StatusBar_renderText("playing | F1 - return to designer");
    }
    void onStart(atLevelDesigner_Data &data) override
    {
        data.player.rect = lwp::Rectangle::build_BottomCenter(data.levelData.playerPos, {atDefaults::playerW_lwp, atDefaults::playerH_lwp});
    }
    void onExit(atLevelDesigner_Data &) override
    {
    }
};