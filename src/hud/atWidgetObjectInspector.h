#pragma once
#include "raylib.h"
#include "raygui.h"
#include "../atLevel/atObject.h"
#include "atStringTemplate.h"
#include <vector>

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

struct atWidgetDropDownBox
{
    atStringTemplate<32> text;
    int active = 0;
    bool editMode = false;
    void draw(Rectangle area)
    {
        if (GuiDropdownBox(area, text.data, &active, editMode) != RESULT_NONE)
            editMode = !editMode;
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
    atWidgetDropDownBox widgetDropdownBoxCollection;
    atWidgetDropDownBox widgetDropdownBoxLayer;
    uint32_t objId = 0;
    bool isFirstObject = true;
    void updateDataForNewObject(atObjectRef &objRef)
    {
        atObject *obj = objRef.resolve();
        if (isFirstObject || objId != obj->id)
        {
            isFirstObject = false;
            objId = obj->id;
            widgetDropdownBoxCollection.active = (int)objRef.collectionIndex;
            widgetDropdownBoxLayer.active = (int)objRef.layerIndex;
        }
    }

    void draw(atObjectRef &objRef, const Rectangle &areaFull, atWidgetTheme &theme)
    {
        atObject *objPointer = objRef.resolve();
        atObjectsCollection *objCollection = objRef.resolveCollection();
        if (!objPointer || !objCollection)
            return;
        updateDataForNewObject(objRef);
        atObject &obj = *objPointer;

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

        if (widgetDropdownBoxCollection.text.length() == 0)
        {
            atObjectsCollectionLayer *layer = objRef.resolveLayer();
            if (layer)
            {
                for (auto &collection : layer->collections())
                {
                    widgetDropdownBoxCollection.text.append(collection->tag.data);
                    widgetDropdownBoxCollection.text.append(";");
                }
                widgetDropdownBoxCollection.text.data[widgetDropdownBoxCollection.text.length() - 1] = 0;
            }
        }

        if (widgetDropdownBoxLayer.text.length() == 0)
        {
            for (atObjectsCollectionLayer &layer : *objRef.layers)
            {
                widgetDropdownBoxLayer.text.append(layer.tag.data);
                widgetDropdownBoxLayer.text.append(";");
            }
            widgetDropdownBoxLayer.text.data[widgetDropdownBoxLayer.text.length() - 1] = 0;
        }

        drawTextNewLine("Layer: ");
        areaForNextElementTryHorizontal();
        Rectangle areaForEditLayer = areaForEdit;
        drawTextNewLine("Collection: ");
        areaForNextElementTryHorizontal();
        widgetDropdownBoxCollection.draw(areaForEdit);
        widgetDropdownBoxLayer.draw(areaForEditLayer); // reverse order not to overlay on dropdown

        GuiSetStyle(DEFAULT, TEXT_SIZE, textSizeBefore);
    }
};

class atWidgetObjectInspector
{
public:
    atWidgetObjectProperties widgetTransform;
    atWidgetTheme theme;
    void draw(std::vector<atObjectRef> &objectsRef, Rectangle panelRect)
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

        atObjectRef &objRef = objectsRef[0];

        if (!objRef.resolve())
            return;

        widgetTransform.draw(objRef, {panelRect.x + theme.gap.x, y, panelRect.width, panelRect.height}, theme);
        size_t selectedCollectionIndex = (size_t)widgetTransform.widgetDropdownBoxCollection.active;
        if (selectedCollectionIndex != objRef.collectionIndex)
        {
            atObjectLayers_moveObjectToCollection(objRef, selectedCollectionIndex);
            Console::log("[move collection] from {} to {}\n", objRef.collectionIndex, selectedCollectionIndex);
        }

        size_t selectedLayerIndex = (size_t)widgetTransform.widgetDropdownBoxLayer.active;
        if (selectedLayerIndex != objRef.layerIndex)
        {
            Console::log("[move layer] from {} to {}\n", objRef.layerIndex, selectedLayerIndex);
            atObjectLayers_moveObjectToLayer(objRef, selectedLayerIndex);
        }
    }
};
