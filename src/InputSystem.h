#pragma once
#include "raylib.h"
#include <optional>

enum class InputSystemDirection
{
    left,
    right
};

enum class InputSystemAction
{
    jump,
    layerNext,
    layerPrev
};

struct InputSystemResult
{
    std::optional<InputSystemDirection> direction = std::nullopt;
    std::optional<InputSystemAction> action = std::nullopt;
};

class InputSystem
{
public:
    InputSystemResult update()
    {
        InputSystemResult result;

        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_SPACE))
            result.action = InputSystemAction::jump;
        else if (IsKeyPressed(KEY_Q))
            result.action = InputSystemAction::layerPrev;
        else if (IsKeyPressed(KEY_E))
            result.action = InputSystemAction::layerNext;

        bool keyLeft = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
        bool keyRight = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);

        if (keyLeft != keyRight)
        {
            if (keyLeft)
                result.direction = InputSystemDirection::left;
            else
                result.direction = InputSystemDirection::right;
        }

        return result;
    }
};