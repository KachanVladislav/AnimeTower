#pragma once
#include "../atLevel/atLevelData.h"
#include "../atRender/atRenderLevel.h"
#include "../atConvert_lwpToRaylib.h"
#include "raygui.h"

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

struct atLevelDesignerModeInterface
{
    virtual void beforeRender(atLevelDesigner_Data &data) = 0;
    virtual void afterRender(atLevelDesigner_Data &data) = 0;
    virtual void render_2D(atLevelDesigner_Data &data) = 0;
    virtual void render_Screen(atLevelDesigner_Data &data) = 0;
    virtual void onStart(atLevelDesigner_Data &data) = 0;
    virtual void onExit(atLevelDesigner_Data &data) = 0;
};

/// DUMPSTER
static inline void StatusBar_renderText(const char *text)
{
    GuiLabel({10, 10, 200, 10}, text);
}

static inline lwp::Vector2f getMousePos_lwp(const Camera2D &camera)
{
    Vector2 mouseScreen = GetMousePosition();
    Vector2 mouse_raylib = GetScreenToWorld2D(mouseScreen, camera);
    return atConvert_RaylibTolwp::position(mouse_raylib);
}

static inline lwp::Rectangle buildRectangleByPos_lwp(const lwp::Vector2f pos1, const lwp::Vector2f pos2)
{
    lwp::Rectangle rect;
    rect.x = (pos1.x < pos2.x) ? pos1.x : pos2.x;
    rect.y = (pos1.y < pos2.y) ? pos1.y : pos2.y;
    rect.width = fabsf(pos1.x - pos2.x);
    rect.height = fabsf(pos1.y - pos2.y);
    return rect;
}