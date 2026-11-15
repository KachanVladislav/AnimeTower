#pragma once
#include "atConvert_lwpToRaylib.h"

struct atCamera
{
    static void setCameraToScreen_lwp(Camera2D &camera, float screenWidth_lwp)
    {
        const float screenWidth = (float)GetScreenWidth();
        const float screenHeight = (float)GetScreenHeight();
        camera.offset = Vector2{screenWidth / 2.0f, screenHeight / 2.0f};
        camera.rotation = 0.0f;

        camera.zoom = screenWidth / screenWidth_lwp;
    }
    static void setCameraTarget_lwp(Camera2D &camera, const lwp::Vector2f targetPos)
    {
        camera.target = atConvert_lwpToRaylib::position(targetPos);
    }
};
