#pragma once
#include "raylib.h"
#include "InputSystem.h"

struct WidgetInputSystemTestSettings
{
    int spacing = 0;
    int fontSize[3] {};
    Vector2 keyPos[3]{};
    Vector2 keySize;
    Vector2 textPos[3]{};
    Font font;
    const char *keyNames[3] = {"<", "jmp", ">"};
    static WidgetInputSystemTestSettings buildDefault(Vector2 pos, Font font);
};

class WidgetInputSystemTest
{

public:
    WidgetInputSystemTestSettings settings;
    void draw(const InputSystemResult &input)
    {
        constexpr Color COLOR_NOT_PRESSED = GRAY;
        constexpr Color COLOR_PRESSED = LIME;
        constexpr Color COLOR_TEXT_NOT_PRESSED = GREEN;
        constexpr Color COLOR_TEXT_PRESSED = YELLOW;
        Color colorsFill[3]{COLOR_NOT_PRESSED, COLOR_NOT_PRESSED, COLOR_NOT_PRESSED};
        Color colorsText[3]{COLOR_TEXT_NOT_PRESSED, COLOR_TEXT_NOT_PRESSED, COLOR_TEXT_NOT_PRESSED};

        if (input.direction)
        {
            if (*input.direction == InputSystemDirection::left)
            {
                colorsFill[0] = COLOR_PRESSED;
                colorsText[0] = COLOR_TEXT_PRESSED;
            }
            else
            {
                colorsFill[2] = COLOR_PRESSED;
                colorsText[2] = COLOR_TEXT_PRESSED;
            }
        }
        if (input.action)
        {
            colorsFill[1] = COLOR_PRESSED;
            colorsText[1] = COLOR_TEXT_PRESSED;
        }

        for (int i = 0; i < 3; i++)
        {
            DrawRectangleV(settings.keyPos[i], settings.keySize, colorsFill[i]);
            DrawTextEx(settings.font, settings.keyNames[i], settings.textPos[i], settings.fontSize[i], 1, colorsText[i]);
        }
    }
};

WidgetInputSystemTestSettings WidgetInputSystemTestSettings::buildDefault(Vector2 pos, Font font)
{
    float keyWidth = 50.f;
    WidgetInputSystemTestSettings res;
    res.keySize = Vector2{keyWidth, keyWidth};

    res.fontSize[0] = 30;
    res.fontSize[1] = 25;
    res.fontSize[2] = 30;
    res.spacing = 5;
    res.font = font;
    for (int i = 0; i < 3; i++)
    {
        res.keyPos[i].x = pos.x + i * (keyWidth + res.spacing);
        res.keyPos[i].y = pos.y;
        Vector2 textSize = MeasureTextEx(res.font, res.keyNames[i], res.fontSize[i], 1);
        res.textPos[i].x = res.keyPos[i].x + keyWidth / 2 - textSize.x / 2;
        res.textPos[i].y = pos.y + keyWidth / 2 - res.fontSize[i] / 2;
    }
    return res;
}