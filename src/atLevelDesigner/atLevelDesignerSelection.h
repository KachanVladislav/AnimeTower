#pragma once
#include "atLevelDesignerMode_Interface.h"
#include "../Console.h"
#include <algorithm>
#include <memory>

struct atLevelDesignerSelection
{
    std::vector<int> selectedPlatformIndices;
    bool isAreaSelecting = false;
    lwp::Vector2f areaSelectStart;
    lwp::Vector2f areaSelectEnd;

    void update(atLevelDesigner_Data &data, atLevelDesignerOp_Executer& opExecuter)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            isAreaSelecting = true;
            areaSelectStart = getMousePos_lwp(data.camera);
            areaSelectEnd = areaSelectStart;
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && isAreaSelecting)
        {
            areaSelectEnd = getMousePos_lwp(data.camera);
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && isAreaSelecting)
        {
            isAreaSelecting = false;
            lwp::Rectangle selectRect = buildRectangleByPos_lwp(areaSelectStart, areaSelectEnd);
            if (selectRect.width < 0.01f && selectRect.height < 0.01f)
            {
                selectedPlatformIndices.clear();
                for (size_t i = 0; i < data.levelData.horizontalPlatforms_lwp.size(); ++i)
                {
                    const lwp::Rectangle &rect = data.levelData.horizontalPlatforms_lwp[i];
                    lwp::Vector2f mousePos = areaSelectStart;
                    if (mousePos.x >= rect.x && mousePos.x <= rect.x + rect.width &&
                        mousePos.y >= rect.y && mousePos.y <= rect.y + rect.height)
                    {
                        selectedPlatformIndices.push_back(static_cast<int>(i));
                        Console::log("selected platform [{}]: x={} y={} w={} h={}\n",
                                     i, rect.x, rect.y, rect.width, rect.height);
                        break;
                    }
                }
            }
            else
            {
                selectedPlatformIndices.clear();
                Console::log("area selected:\n");
                for (size_t i = 0; i < data.levelData.horizontalPlatforms_lwp.size(); ++i)
                {
                    const lwp::Rectangle &rect = data.levelData.horizontalPlatforms_lwp[i];
                    if (rect.x + rect.width >= selectRect.x && rect.x <= selectRect.x + selectRect.width &&
                        rect.y + rect.height >= selectRect.y && rect.y <= selectRect.y + selectRect.height)
                    {
                        selectedPlatformIndices.push_back(static_cast<int>(i));
                        Console::log("  platform [{}]: x={} y={} w={} h={}\n",
                                     i, rect.x, rect.y, rect.width, rect.height);
                    }
                }
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            selectedPlatformIndices.clear();
        }
        if (IsKeyPressed(KEY_D) && !selectedPlatformIndices.empty())
        {
            deleteSelected(data, opExecuter);
        }
        
    }

    void render(atLevelDesigner_Data &data)
    {
        for (int idx : selectedPlatformIndices)
        {
            const lwp::Rectangle &rect_lwp = data.levelData.horizontalPlatforms_lwp[idx];
            Rectangle rect_raylib = atConvert_lwpToRaylib::rectangle(rect_lwp);
            DrawRectangleLinesEx(rect_raylib, 0.1f, GREEN);
        }
        if (isAreaSelecting)
        {
            lwp::Rectangle selectRect_lwp = buildRectangleByPos_lwp(areaSelectStart, areaSelectEnd);
            Rectangle selectRect_raylib = atConvert_lwpToRaylib::rectangle(selectRect_lwp);
            DrawRectangleLinesEx(selectRect_raylib, 0.05f, BLUE);
        }
    }

    void clear()
    {
        selectedPlatformIndices.clear();
    }

private:
    void deleteSelected(atLevelDesigner_Data &data, atLevelDesignerOp_Executer& opExecuter)
    {
        std::vector<lwp::Rectangle> platforms;
        std::vector<int> indices = selectedPlatformIndices;
        std::sort(indices.begin(), indices.end());
        for (int idx : indices)
        {
            platforms.push_back(data.levelData.horizontalPlatforms_lwp[idx]);
        }
        auto op = std::make_unique<atLevelDesignerOp_DeletePlatforms>(std::move(platforms), std::move(indices));
        opExecuter.execute(std::move(op), data);
        selectedPlatformIndices.clear();
    }
};
