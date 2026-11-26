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
        atObject *player = data.levelData.playerRef.resolve();
        if (!player)
            return;

        int layerInc = 0; // prototyping layers change
        if (input.action == InputSystemAction::layerNext)
            layerInc = 1;
        else if (input.action == InputSystemAction::layerPrev)
            layerInc = -1;
        if (layerInc != 0)
        {
            int resultNum = (int)data.levelData.currentLayerNum + layerInc;
            if (resultNum >= 0 && resultNum < data.levelData.layers.size())
            {
                data.levelData.currentLayerNum = resultNum;
                atObjectLayers_moveObjectToLayer(data.levelData.playerRef, resultNum);

                atRenderObjectRef renderObjRef(&data.levelData.layersRenderData, player->id); 
                if (renderObjRef.resolve())
                {
                    Console::log("[move render layer] from {} to {}\n", renderObjRef.layerIndex, resultNum);
                    atObjectLayers_moveRenderObjectToLayer(renderObjRef, resultNum);
                }

                player = data.levelData.playerRef.resolve();
            }
        }

        float dT = GetFrameTime();
        atPhysics::applyInputToRigidBody(*player, input);
        if (input.direction)
            player->isFlipX = (*input.direction == InputSystemDirection::right);

        for (auto &layer : data.levelData.layers)
            atPhysics::updatePhysics(layer.colRigid.vec, layer.colStatic.vec, dT);
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
        atObject *player = data.levelData.playerRef.resolve();
        if (!player)
            return;
        player->transform.pos = data.levelData.playerDefaultPos;
        player->rigidBody.speed = lwp::Vector2f{0, 0};
    }
    void onExit(atLevelDesigner_Data &) override
    {
    }
};