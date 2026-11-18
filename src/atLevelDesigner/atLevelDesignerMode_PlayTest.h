#pragma once
#include "atLevelDesignerMode_Interface.h"
#include "../InputSystem.h"
#include "../atPhysics.h"
#include "../atDefaults.h"

struct atLevelDesignerMode_PlayTest : public atLevelDesignerModeInterface
{
    InputSystem inputSystem;
    void beforeRender(atLevelDesigner_Data &data, atLevelDesignerOp_Executer &) override
    {
        InputSystemResult input = inputSystem.update();
        atObject* player = data.levelData.playerRef.resolve();
        if(!player)
            return;
        
        float dT = GetFrameTime();
        atPhysics::applyInputToRigidBody(*player, input);
        atPhysics::updatePhysics(data.levelData.objectsRigid, data.levelData.objectsStatic, dT);
        if (input.direction)
            player->isFlipX = (*input.direction == InputSystemDirection::right);
    }
    void afterRender(atLevelDesigner_Data &, atLevelDesignerOp_Executer &) override
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
        atObject* player = data.levelData.playerRef.resolve();
        if(!player)
            return;
        player->transform.pos = data.levelData.playerDefaultPos;
        player->rigidBody.speed = lwp::Vector2f{0, 0};
    }
    void onExit(atLevelDesigner_Data &) override
    {
    }
};