#pragma once
#include <span>
#include <vector>
#include "raylib.h"
#include "lwp/lwp.h"

struct atConvert_lwpToRaylib
{
    static Vector2 position(const lwp::Vector2f &pos_lwp)
    {
        return Vector2{pos_lwp.x, -pos_lwp.y};
    }
    static Rectangle rectangle(const lwp::Rectangle &rect_lwp)
    {
        Vector2 pos = position(rect_lwp.getPos_TopLeft());
        return Rectangle{pos.x, pos.y, rect_lwp.width, rect_lwp.height};
    }

    static std::vector<Rectangle> buildRectsVector(std::span<const lwp::Rectangle> rects_lwp)
    {
        std::vector<Rectangle> rects_raylib;
        rects_raylib.reserve(rects_lwp.size());
        for (const auto &rect_lwp : rects_lwp)
            rects_raylib.push_back(atConvert_lwpToRaylib::rectangle(rect_lwp));
        return rects_raylib;
    }
};
