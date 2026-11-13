#pragma once
#include <span>
#include "lwp/lwp.h"
#include "InputSystem.h"

struct atPhysics
{
    static inline const float g = 10.f;
    static inline const float MAX_SPEED = 5;
    static inline const float JUMP_SPEED = 5;

    static void applyInputToRigidBody(lwp::RigidBody2D &body, const InputSystemResult &input)
    {
        if (body.isOnFloor())
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
    static void updatePhysics(lwp::RigidBody2D &player, std::span<lwp::Rectangle> staticObjects, float dT)
    {
        float playerBottomBefore = player.rect.getY_Bottom();

        player.speed.y = player.speed.y - g * dT;
        player.applySpeedToBoxPos(dT);
        player.onFloor = false;

        float playerBottom = player.rect.getY_Bottom();
        for (const auto box : staticObjects)
        {
            // floor collision
            if (playerBottomBefore >= box.getY_Top() && playerBottom <= box.getY_Top()) // y-level
            {
                bool notAligned_x = player.rect.getX_Right() < box.getX_Left() || player.rect.getX_Left() > box.getX_Right();
                if (!notAligned_x) // x-level
                    player.setOnFloor(box.getY_Top());
            }

            // same y-level
            if (player.rect.getY_Bottom() < box.getY_Top() && player.rect.getY_Top() > box.getY_Top())
            {
                // right collision
                if (player.rect.getX_Right() > box.getX_Left() && player.rect.getX_Left() < box.getX_Left())
                {
                    if(player.speed.x > 0)
                        player.rect.setX_Right(box.getX_Left());
                }

                // left collision
                if (player.rect.getX_Left() < box.getX_Right() && player.rect.getX_Right() > box.getX_Right())
                {
                    if(player.speed.x < 0)
                        player.rect.setX_Left(box.getX_Right());
                }
            }
        }
    }
};