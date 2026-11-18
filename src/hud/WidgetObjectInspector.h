#pragma once
#include "raylib.h"
#include "raygui.h"
#include "../atLevel/atObject.h"
#include <vector>
#include <string>
#include <format>
#include <cstring>
#include <cstdlib>

struct WidgetObjectInspectorSettings
{
    Vector2 panelPos{0, 0};
    float panelWidth = 280.f;
    float panelHeight = 450.f;
    float padding = 10.f;
    float fontSize = 14.f;
    float lineHeight = 24.f;
    float labelWidth = 70.f;
    float fieldHeight = 22.f;
    Font font;
    Color panelColor = {40, 40, 40, 200};
    Color textColor = RAYWHITE;
    Color headerColor = YELLOW;
    Color labelColor = LIGHTGRAY;

    static WidgetObjectInspectorSettings buildDefault(Font font);
};
//TODO NEED REDO
class WidgetObjectInspector
{
public:
    WidgetObjectInspectorSettings settings;

    char posXBuffer[32] = "";
    char posYBuffer[32] = "";
    char widthBuffer[32] = "";
    char heightBuffer[32] = "";
    char idBuffer[32] = "";
    char flipXBuffer[32] = "";
    
    char prevPosXBuffer[32] = "";
    char prevPosYBuffer[32] = "";
    char prevWidthBuffer[32] = "";
    char prevHeightBuffer[32] = "";
    
    uint32_t lastSelectedId = 0;
    bool buffersInitialized = false;

    void draw(const std::vector<uint32_t> &selectedIds, std::vector<atObject> &objects, Rectangle panelRect)
    {
        DrawRectangleRec(panelRect, settings.panelColor);
        DrawRectangleLinesEx(panelRect, 1.f, GRAY);

        float y = panelRect.y + settings.padding;
        
        DrawTextEx(settings.font, "Object Inspector", 
                   {panelRect.x + settings.padding, y}, 
                   settings.fontSize + 2, 1, settings.headerColor);
        y += settings.lineHeight * 1.5f;

        if (selectedIds.empty())
        {
            DrawTextEx(settings.font, "No selection",
                       {panelRect.x + settings.padding, y},
                       settings.fontSize, 1, GRAY);
            buffersInitialized = false;
            return;
        }

        uint32_t currentId = selectedIds[0];
        const atObject *firstObj = findById(currentId, objects);
        if (!firstObj) return;

        bool selectionChanged = !buffersInitialized || lastSelectedId != currentId;
        
        if (selectionChanged)
        {
            lastSelectedId = currentId;
            buffersInitialized = true;
            auto rect = firstObj->getColliderRect();
            snprintf(posXBuffer, sizeof(posXBuffer), "%.2f", rect.x);
            snprintf(posYBuffer, sizeof(posYBuffer), "%.2f", rect.y);
            snprintf(widthBuffer, sizeof(widthBuffer), "%.2f", rect.width);
            snprintf(heightBuffer, sizeof(heightBuffer), "%.2f", rect.height);
            memcpy(prevPosXBuffer, posXBuffer, sizeof(prevPosXBuffer));
            memcpy(prevPosYBuffer, posYBuffer, sizeof(prevPosYBuffer));
            memcpy(prevWidthBuffer, widthBuffer, sizeof(prevWidthBuffer));
            memcpy(prevHeightBuffer, heightBuffer, sizeof(prevHeightBuffer));
        }
        else
        {
            applyChanges(selectedIds, objects);
        }

        auto rect = firstObj->getColliderRect();
        bool allSamePos = true, allSameSize = true;

        for (size_t i = 1; i < selectedIds.size(); i++)
        {
            const atObject *obj = findById(selectedIds[i], objects);
            if (!obj) continue;
            auto r = obj->getColliderRect();
            if (r.x != rect.x || r.y != rect.y) allSamePos = false;
            if (r.width != rect.width || r.height != rect.height) allSameSize = false;
        }

        snprintf(idBuffer, sizeof(idBuffer), "%u", firstObj->id);
        y = drawField("ID:", idBuffer, y, panelRect.x + settings.padding, true);

        if (!allSamePos)
        {
            snprintf(posXBuffer, sizeof(posXBuffer), "---");
            snprintf(posYBuffer, sizeof(posYBuffer), "---");
        }
        y = drawField("Pos X:", posXBuffer, y, panelRect.x + settings.padding, false);
        y = drawField("Pos Y:", posYBuffer, y, panelRect.x + settings.padding, false);

        if (!allSameSize)
        {
            snprintf(widthBuffer, sizeof(widthBuffer), "---");
            snprintf(heightBuffer, sizeof(heightBuffer), "---");
        }
        y = drawField("Width:", widthBuffer, y, panelRect.x + settings.padding, false);
        y = drawField("Height:", heightBuffer, y, panelRect.x + settings.padding, false);

        snprintf(flipXBuffer, sizeof(flipXBuffer), "%s", firstObj->isFlipX ? "true" : "false");
        y = drawField("FlipX:", flipXBuffer, y, panelRect.x + settings.padding, true);
    }

private:
    atObject *findById(uint32_t id, std::vector<atObject> &objects) const
    {
        for (auto &obj : objects)
            if (obj.id == id) return &obj;
        return nullptr;
    }

    void applyChanges(const std::vector<uint32_t> &selectedIds, std::vector<atObject> &objects)
    {
        // Не применяем если буферы содержат "---" (множественное выделение с разными значениями)
        if (strcmp(posXBuffer, "---") == 0 || strcmp(posYBuffer, "---") == 0 ||
            strcmp(widthBuffer, "---") == 0 || strcmp(heightBuffer, "---") == 0)
        {
            // Обновляем prev чтобы не триггерить изменения когда буферы вернутся к нормальным значениям
            memcpy(prevPosXBuffer, posXBuffer, sizeof(prevPosXBuffer));
            memcpy(prevPosYBuffer, posYBuffer, sizeof(prevPosYBuffer));
            memcpy(prevWidthBuffer, widthBuffer, sizeof(prevWidthBuffer));
            memcpy(prevHeightBuffer, heightBuffer, sizeof(prevHeightBuffer));
            return;
        }

        bool posXChanged = strcmp(posXBuffer, prevPosXBuffer) != 0;
        bool posYChanged = strcmp(posYBuffer, prevPosYBuffer) != 0;
        bool widthChanged = strcmp(widthBuffer, prevWidthBuffer) != 0;
        bool heightChanged = strcmp(heightBuffer, prevHeightBuffer) != 0;

        if (!posXChanged && !posYChanged && !widthChanged && !heightChanged)
            return;

        float newX = posXChanged ? (float)atof(posXBuffer) : 0;
        float newY = posYChanged ? (float)atof(posYBuffer) : 0;
        float newW = widthChanged ? (float)atof(widthBuffer) : 0;
        float newH = heightChanged ? (float)atof(heightBuffer) : 0;

        for (uint32_t id : selectedIds)
        {
            atObject *obj = findById(id, objects);
            if (!obj) continue;

            if (posXChanged || posYChanged)
            {
                obj->transform.pos.x = posXChanged ? newX : obj->transform.pos.x;
                obj->transform.pos.y = posYChanged ? newY : obj->transform.pos.y;
            }
            if (widthChanged || heightChanged)
            {
                obj->collider.size.x = widthChanged ? newW : obj->collider.size.x;
                obj->collider.size.y = heightChanged ? newH : obj->collider.size.y;
            }
        }

        memcpy(prevPosXBuffer, posXBuffer, sizeof(prevPosXBuffer));
        memcpy(prevPosYBuffer, posYBuffer, sizeof(prevPosYBuffer));
        memcpy(prevWidthBuffer, widthBuffer, sizeof(prevWidthBuffer));
        memcpy(prevHeightBuffer, heightBuffer, sizeof(prevHeightBuffer));
    }

    float drawField(const char *label, char *value, float y, float x, bool readOnly) const
    {
        float labelX = x;
        float fieldX = x + settings.labelWidth;
        float fieldWidth = settings.panelWidth - settings.labelWidth - settings.padding * 2;

        DrawTextEx(settings.font, label, {labelX, y + 3}, settings.fontSize, 1, settings.labelColor);
        
        Rectangle fieldRect{fieldX, y, fieldWidth, settings.fieldHeight};
        
        if (readOnly)
        {
            DrawRectangleRec(fieldRect, {60, 60, 60, 255});
            DrawRectangleLinesEx(fieldRect, 1.f, DARKGRAY);
            DrawTextEx(settings.font, value, {fieldX + 5, y + 3}, settings.fontSize, 1, GRAY);
        }
        else
        {
            GuiTextBox(fieldRect, value, 32, false);
        }
        
        return y + settings.lineHeight;
    }
};

WidgetObjectInspectorSettings WidgetObjectInspectorSettings::buildDefault(Font font)
{
    WidgetObjectInspectorSettings res;
    res.font = font;
    return res;
}
