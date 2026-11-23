#pragma once
#include "atWidgetObjectInspector.h"
#include "atLevel/atLevelData.h"
#include <ranges>

struct atWidgetLayersOverview
{
    std::vector<atStringTemplate<32>> stringsLayers;
    std::vector<char *> stringPointers;
    atWidgetTheme theme;
    atStringTemplate<32> stringName;
    bool stringNameEditMode = false;

    int listScrollIndex = 0;
    int listItemActive = -1;
    int listItemFocused = -1;
    int lastItemActive = -1;

    void draw(atLevelData &levelData, Rectangle areaFull)
    {
        int defaultTextSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
        GuiSetStyle(DEFAULT, TEXT_SIZE, theme.fontSize);

        DrawRectangleRec(areaFull, theme.panelColor);
        DrawRectangleLinesEx(areaFull, 1.f, GRAY);
        const Rectangle area = atWidgetStyle::areaAfterSpacing(areaFull, theme.padding);

        std::vector<atObjectsCollectionLayer> &colLayers = levelData.layers;
        const size_t size = levelData.layers.size();

        if (stringsLayers.size() != size)
        {
            stringsLayers.resize(size);
            stringPointers.resize(size);
            for (auto [ptr, str] : std::views::zip(stringPointers, stringsLayers))
                ptr = str.data;
        }
        for (auto [text, layer] : std::views::zip(stringsLayers, colLayers))
        {
            text = layer.tag;
        }

        float requiredHeight = (GuiGetStyle(DEFAULT, TEXT_SIZE) + theme.gap.y) * size;
        Rectangle areaList = area;
        areaList.height = requiredHeight < area.height ? requiredHeight : area.height;
        GuiListViewEx(areaList, stringPointers.data(), (int)size, &listScrollIndex, &listItemActive, &listItemFocused);

        if (lastItemActive != listItemActive)
        {
            lastItemActive = listItemActive;
            if (listItemActive >= 0 && listItemActive < size)
            {
                Console::log("[LAYER LESECT] from {}:{} to {}:{}\n", levelData.currentLayerNum,
                             levelData.layers[levelData.currentLayerNum].tag.data,
                             listItemActive,
                             levelData.layers[listItemActive].tag.data);
                levelData.currentLayerNum = listItemActive;
            }
        }

        Rectangle areaText = area;
        areaText.y = area.y + areaList.height + theme.gap.y;
        areaText.height = GuiGetStyle(DEFAULT, TEXT_SIZE) + theme.gap.y;
        if (GuiTextBox(areaText, stringName.data, (int)stringName.capacity(), stringNameEditMode) != RESULT_NONE)
            stringNameEditMode = !stringNameEditMode;

        Rectangle areaBtn1 = areaText;
        areaBtn1.y = areaText.y + areaText.height + theme.gap.y;
        areaBtn1.width = area.width / 2 - theme.gap.x;
        if(GuiButton(areaBtn1, "Add"))
        {
            if(stringName.length() != 0)
            {
                levelData.layers.push_back({});
                levelData.layers.back().tag = stringName;
            }
        }
        Rectangle areaBtn2 = areaBtn1;
        areaBtn2.x = areaBtn2.x + area.width / 2;
        if(GuiButton(areaBtn2, "Rename"))
        {
            if(stringName.length() != 0)
            {
                levelData.currentLayer().tag = stringName;
            }
        }

        Rectangle areaBtn3 = areaBtn1;
        areaBtn3.y = areaBtn1.y + areaBtn1.height + theme.gap.y;
        if(GuiButton(areaBtn3, "Delete"))
        {
            if(levelData.layers.size() > 1 && levelData.currentLayerNum != 0)
            {
                levelData.layers.erase(levelData.layers.begin() + levelData.currentLayerNum);
                if(levelData.currentLayerNum >= levelData.layers.size())
                    levelData.currentLayerNum = 0;
            }
        }

        GuiSetStyle(DEFAULT, TEXT_SIZE, defaultTextSize);

    }
};
