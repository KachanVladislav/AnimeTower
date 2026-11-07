#pragma once
#include "lwp/lwp.h"
#include "raylib.h"

struct atCamera
{
    Camera2D camera{};
    void initialize(float screenWidth, float screenHeight, float screenWidth_lwp)
    {
        camera.target = Vector2{0, 0};
        camera.offset = Vector2{screenWidth / 2.0f, screenHeight / 2.0f};
        camera.rotation = 0.0f;

        camera.zoom = screenWidth / screenWidth_lwp;
    }
    void setTarget_lwp(const lwp::Vector2f pos)
    {
        camera.target.x = pos.x;
        camera.target.y = -pos.y;
    }
};