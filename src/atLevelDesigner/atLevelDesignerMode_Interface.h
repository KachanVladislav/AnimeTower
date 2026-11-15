#pragma once
#include "atLevelDesigner_Data.h"
#include "atLevelDesignerOp_Executer.h"
#include "raygui.h"
#include <memory>

struct atLevelDesignerModeInterface
{
    virtual void beforeRender(atLevelDesigner_Data &data, atLevelDesignerOp_Executer &opExecuter) = 0;
    virtual void afterRender(atLevelDesigner_Data &data, atLevelDesignerOp_Executer &opExecuter) = 0;
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

#include <cmath>
static inline lwp::Rectangle buildRectangleByPos_lwp(const lwp::Vector2f pos1, const lwp::Vector2f pos2)
{
    lwp::Rectangle rect;
    rect.x = (pos1.x < pos2.x) ? pos1.x : pos2.x;
    rect.y = (pos1.y < pos2.y) ? pos1.y : pos2.y;
    rect.width = fabsf(pos1.x - pos2.x);
    rect.height = fabsf(pos1.y - pos2.y);
    return rect;
}