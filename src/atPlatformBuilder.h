#pragma once
#include "lwp/lwp.h"

struct atPlatformBuilder
{
    float platformHeight;
    atPlatformBuilder(float platformHeight) : platformHeight(platformHeight) {}
    lwp::Rectangle buildHorizontalRight(lwp::Vector2f pos, float width)
    {
        return lwp::Rectangle::build_UpLeft(pos, {width, platformHeight});
    }
    lwp::Rectangle buildHorizontalLeft(lwp::Vector2f pos, float width)
    {
        return buildHorizontalRight(lwp::Vector2f{pos.x - width, pos.y}, width);
    }
};