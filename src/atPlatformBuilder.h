#pragma once
#include "lwp/lwp.h"

struct atPlatformBuilder
{
    static inline const float PLATFORM_WIDTH = 0.5f;
    lwp::Rectangle buildHorizontalRight(lwp::Vector2f pos, float width)
    {
        return lwp::Rectangle::build_UpLeft(pos, {width, PLATFORM_WIDTH});
    }
    lwp::Rectangle buildHorizontalLeft(lwp::Vector2f pos, float width)
    {
        return buildHorizontalRight(lwp::Vector2f{pos.x - width, pos.y}, width);
    }
};