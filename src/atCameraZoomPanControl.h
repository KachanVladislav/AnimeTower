#pragma once
#include "atCamera.h"
#include "atDefaults.h"

struct atCameraZoomPanControl
{
    float screenWidth_lwp = atDefaults::screenWidth_lwp;
    enum class Mode
    {
        follow,
        pan
    } mode = Mode::follow;
    lwp::Vector2f cameraPanStartTargetPos_lwp;
    Vector2 cameraPanMouseStartPos_Screen;
    void update(Camera2D &camera, lwp::Rectangle rectToFollow, Rectangle vp)
    {
        float mouseMove = GetMouseWheelMove();
        if (mouseMove != 0)
        {
            screenWidth_lwp -= mouseMove;
            if (screenWidth_lwp < 1.f)
                screenWidth_lwp = 1.f;
        }
        atCamera::setCameraToScreen_lwp(camera, screenWidth_lwp, vp);

        if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
        {
            mode = Mode::pan;
            cameraPanMouseStartPos_Screen = GetMousePosition();
            cameraPanStartTargetPos_lwp = atConvert_RaylibTolwp::position(camera.target);
        }

        if (mode == Mode::pan && IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
        {
            const Vector2 mousePos_Screen = GetMousePosition();
            const float ratio = screenWidth_lwp / GetScreenWidth();
            Vector2 delta_raylib{cameraPanMouseStartPos_Screen.x - mousePos_Screen.x,
                                 cameraPanMouseStartPos_Screen.y - mousePos_Screen.y};
            lwp::Vector2f delta_lwp = atConvert_RaylibTolwp::position(delta_raylib) * ratio;
            atCamera::setCameraTarget_lwp(camera, cameraPanStartTargetPos_lwp + delta_lwp);
        }
        if (mode == Mode::follow)
            atCamera::setCameraTarget_lwp(camera, rectToFollow.getCenter());

        if (IsKeyPressed(KEY_R))
            mode = Mode::follow;
    }
};