#pragma once
#include "lwp/lwp.h"

struct atTransform
{
    lwp::Vector2f pos;
};

struct atColliderRect
{
    lwp::Vector2f offset;
    lwp::Vector2f size;
};

struct atRigidBody2D
{
    lwp::Vector2f speed{};
    bool isOnFloor = false;
};

struct atObject
{
    uint32_t id = 0;
    atTransform transform;
    atColliderRect collider;
    atRigidBody2D rigidBody;
    bool isFlipX = false;
    lwp::Vector2f getPos_UpLeft() const { return {transform.pos.x, transform.pos.y + collider.size.y}; }
    lwp::Rectangle getColliderRect() const
    {
        return {transform.pos.x + collider.offset.x,
                transform.pos.y + collider.offset.y,
                collider.size.x,
                collider.size.y};
    }
};