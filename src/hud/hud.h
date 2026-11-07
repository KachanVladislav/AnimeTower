#pragma once
#include "raylib.h"
#include "WidgetInputSystemTest.h"

struct HUD
{
    WidgetInputSystemTest widgetInput;
    void initialize()
    {
        Font font = LoadFontEx("./resource/GoogleSans-VariableFont_GRAD,opsz,wght.ttf", 64, NULL, 0);
        widgetInput.settings = WidgetInputSystemTestSettings::buildDefault(Vector2{10.f, 30.f}, font);
    }
    void draw(const InputSystemResult &input)
    {
        DrawFPS(10, 10);
        widgetInput.draw(input);
    }
};