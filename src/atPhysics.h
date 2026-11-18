#pragma once
#include <span>
#include "lwp/lwp.h"
#include "InputSystem.h"
#include "atLevel/atLevelData.h"

struct atPhysics
{
    static inline const float g = 10.f;
    static inline const float MAX_SPEED = 5;
    static inline const float JUMP_SPEED = 5;

    static void applyInputToRigidBody(atObject &rigidBody, const InputSystemResult &input)
    {
        auto &body = rigidBody.rigidBody;
        if (body.isOnFloor)
        {
            if (input.direction)
            {
                if (*input.direction == InputSystemDirection::right)
                    body.speed.x = MAX_SPEED;
                else
                    body.speed.x = -MAX_SPEED;
            }
            else
            {
                body.speed.x = 0;
            }
            if (input.action)
            {
                if (*input.action == InputSystemAction::jump)
                    body.speed.y = JUMP_SPEED;
            }
        }
    }

    /// @brief apply gravity, resolve collisions
    static void updatePhysics(std::span<atObject> rigidBodies, std::span<atObject> objectsStatic, float dT)
    {
        for (auto &rigidBody : rigidBodies)
        {
            applyGravityToRigidBody(rigidBody, dT);
            for (auto &objectStatic : objectsStatic)
                resolveStaticCollision(rigidBody, objectStatic);
        }
    }

    static void applyGravityToRigidBody(atObject &obj1, float dT)
    {
        obj1.rigidBody.speed.y -= g * dT;
        obj1.transform.pos.x += obj1.rigidBody.speed.x * dT;
        obj1.transform.pos.y += obj1.rigidBody.speed.y * dT;
        obj1.rigidBody.isOnFloor = false;
    }

    static void resolveStaticCollision(atObject &rigid, atObject &stat)
    {
        auto rb = rigid.getColliderRect();
        auto sb = stat.getColliderRect();

        bool overlapX = rb.x < sb.x + sb.width && rb.x + rb.width > sb.x;
        bool overlapY = rb.y < sb.y + sb.height && rb.y + rb.height > sb.y;
        if (!overlapX || !overlapY)
            return;

        float overlapLeft = (rb.x + rb.width) - sb.x;
        float overlapRight = (sb.x + sb.width) - rb.x;
        float overlapBottom = (rb.y + rb.height) - sb.y;
        float overlapTop = (sb.y + sb.height) - rb.y;

        float minOverlapX = std::min(overlapLeft, overlapRight);
        float minOverlapY = std::min(overlapBottom, overlapTop);

        if (minOverlapX < minOverlapY)
        {
            float colliderX = 0;
            if (overlapLeft < overlapRight)
                colliderX = sb.x - rb.width;
            else
                colliderX = sb.x + sb.width;
            // rigid.rigidBody.speed.x = 0;
            rigid.transform.pos.x = colliderX - rigid.collider.offset.x;
        }
        else
        {
            float colliderY = 0;
            if (overlapBottom < overlapTop)
            {
                colliderY = sb.y - rb.height;
            }
            else
            {
                rigid.rigidBody.isOnFloor = true;
                colliderY = sb.y + sb.height;
            }
            rigid.rigidBody.speed.y = 0;
            rigid.transform.pos.y = colliderY - rigid.collider.offset.y;
        }
    }
};