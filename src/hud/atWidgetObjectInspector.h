#pragma once
#include "raylib.h"
#include "raygui.h"
#include "../atLevel/atObject.h"
#include <vector>
#include <cstring>
#include <cstdint>

template <size_t CAPACITY>
struct atStringTemplate
{
    char data[CAPACITY]{};
    size_t capacity() const { return CAPACITY; }

    constexpr atStringTemplate() = default;
    void fromFloat(float value)
    {
        int len = snprintf(data, CAPACITY, "%.2f", value);
        std::memset(&data[len], 0, CAPACITY - len);
    }
    void clear()
    {
        std::memset(&data[0], 0, CAPACITY);
    }
    bool length()
    {
        return strnlen_s(data, CAPACITY);
    }
};

struct atWidgetSpacing
{
    float top{};
    float right{};
    float bottom{};
    float left{};
    static atWidgetSpacing buildEqual(float value)
    {
        atWidgetSpacing res;
        res.top = value;
        res.right = value;
        res.bottom = value;
        res.left = value;
        return res;
    }
};

struct atWidgetStyle
{
    atWidgetSpacing padding;
    float gap;
    static Rectangle areaAfterSpacing(const Rectangle &area, const atWidgetSpacing &padding)
    {
        return Rectangle{area.x + padding.left,
                         area.y + padding.top,
                         area.width - padding.left - padding.right,
                         area.height - padding.top - padding.bottom};
    }
};

struct atWidgetTheme
{
    Vector2 gap;
    atWidgetSpacing padding;
    int fontSize;
    Color panelColor = {40, 40, 40, 200};
    Color headerColor = YELLOW;
    Color textColor = RAYWHITE;
    Font font;
    static atWidgetTheme buildDefault(Font font)
    {
        atWidgetTheme res;
        res.gap.x = 20.f;
        res.gap.y = 20.f;
        res.padding = atWidgetSpacing{10, 10, 10, 10};
        res.fontSize = 20;
        res.font = font;
        return res;
    }
};

struct atWidgetEditFloat
{
    bool isEditingMode = false;
    atStringTemplate<RAYGUI_VALUEBOX_MAX_CHARS + 1> bufferStr;
    float startValue;
    bool needClearOnStartEdit = true;
    void draw(const Rectangle &area, const char *text, float *value)
    {
        if (!isEditingMode)
        {
            startValue = *value;
            bufferStr.fromFloat(*value);
            needClearOnStartEdit = true;
        }
        else if (needClearOnStartEdit)
        {
            bufferStr.clear();
            needClearOnStartEdit = false;
        }

        if (GuiValueBoxFloat(area, text, bufferStr.data, value, isEditingMode) != RESULT_NONE)
            isEditingMode = !isEditingMode;
        if (bufferStr.length() == 0)
            *value = startValue;
    }
};

struct atWidgetObjectProperties
{
    atWidgetEditFloat widgetEditX;
    atWidgetEditFloat widgetEditY;
    atWidgetEditFloat widgetEditColliderOffsetX;
    atWidgetEditFloat widgetEditColliderOffsetY;
    atWidgetEditFloat widgetEditColliderSizeX;
    atWidgetEditFloat widgetEditColliderSizeY;
    void draw(atObject &obj, const Rectangle &areaFull, atWidgetTheme &theme)
    {
        const int textSizeBefore = GuiGetStyle(DEFAULT, TEXT_SIZE);
        GuiSetStyle(DEFAULT, TEXT_SIZE, theme.fontSize);

        const float labelHeight = (float)theme.fontSize + 2;
        const float labelWidth = (float)theme.fontSize * 5;
        const Rectangle area = atWidgetStyle::areaAfterSpacing(areaFull, theme.padding);

        Rectangle areaForEdit{area.x, area.y, labelWidth, labelHeight};

        auto areaForNextElementTryHorizontal = [&]()
        {
            areaForEdit.x += theme.gap.x + labelWidth;
            if (areaForEdit.x + areaForEdit.width > area.x + area.width)
            {
                areaForEdit.x = area.x;
                areaForEdit.y = areaForEdit.y + labelHeight + theme.gap.y;
            }
        };
        auto areaForNextElementVertical = [&]()
        {
            areaForEdit.x = area.x;
            areaForEdit.y = areaForEdit.y + labelHeight + theme.gap.y;
        };

        auto drawTextNewLine = [&](const char *text)
        {
            areaForNextElementVertical();
            DrawText(text, (int)areaForEdit.x, (int)areaForEdit.y, theme.fontSize, theme.textColor);
        };

        DrawText("Transform:", (int)areaForEdit.x, (int)area.y, theme.fontSize, theme.textColor);
        areaForNextElementVertical();
        widgetEditX.draw(areaForEdit, "x", &obj.transform.pos.x);
        areaForNextElementTryHorizontal();
        widgetEditY.draw(areaForEdit, "y", &obj.transform.pos.y);

        
        drawTextNewLine("Collider offset:");
        areaForNextElementVertical();
        widgetEditColliderOffsetX.draw(areaForEdit, "x", &obj.collider.offset.x);
        areaForNextElementTryHorizontal();
        widgetEditColliderOffsetY.draw(areaForEdit, "x", &obj.collider.offset.y);

        drawTextNewLine("Collider Size:");
        areaForNextElementVertical();
        widgetEditColliderSizeX.draw(areaForEdit, "x", &obj.collider.size.x);
        areaForNextElementTryHorizontal();
        widgetEditColliderSizeY.draw(areaForEdit, "y", &obj.collider.size.y);

        GuiSetStyle(DEFAULT, TEXT_SIZE, textSizeBefore);
    }
};

class atWidgetObjectInspector
{
public:
    atWidgetObjectProperties widgetTransform;
    atWidgetTheme theme;
    void draw(std::vector<atObjectRefToVector> &objectsRef, Rectangle panelRect)
    {
        DrawRectangleRec(panelRect, theme.panelColor);
        DrawRectangleLinesEx(panelRect, 1.f, GRAY);

        float y = panelRect.y + theme.gap.y;
        DrawTextEx(GuiGetFont(), "Object Inspector",
                   {panelRect.x + theme.gap.x, y},
                   (float)theme.fontSize + 2, 1, theme.headerColor);

        y += theme.fontSize + 2;
        if (objectsRef.empty())
        {
            DrawTextEx(GuiGetFont(), "No selection",
                       {panelRect.x + theme.gap.x, y},
                       (float)theme.fontSize, 1, GRAY);
            return;
        }
        else if (objectsRef.size() > 1)
        {
            DrawTextEx(GuiGetFont(), "Multiple selection TBD",
                       {panelRect.x + theme.gap.x, y},
                       (float)theme.fontSize, 1, GRAY);
            return;
        }

        atObject *obj = objectsRef[0].resolve();
        if (!obj)
        {
            return;
        }

        widgetTransform.draw(*obj, {panelRect.x + theme.gap.x, y, panelRect.width, panelRect.height}, theme);
    }
};
