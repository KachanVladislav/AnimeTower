#pragma once
#include "atLevelDesignerMode_Interface.h"
#include "../Console.h"
#include <algorithm>
#include <memory>

struct atLevelDesignerSelection
{
    std::vector<uint32_t> selectedPlatformIds;
    bool isAreaSelecting = false;
    lwp::Vector2f areaSelectStart;
    lwp::Vector2f areaSelectEnd;

    void update(atLevelDesigner_Data &data)
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
                selectedPlatformIds.clear();
                for (size_t i = 0; i < data.levelData.objectsStatic.size(); ++i)
                {
                    const auto &obj = data.levelData.objectsStatic[i];
                    const lwp::Rectangle &rect = obj.getColliderRect();
                    lwp::Vector2f mousePos = areaSelectStart;
                    if (mousePos.x >= rect.x && mousePos.x <= rect.x + rect.width &&
                        mousePos.y >= rect.y && mousePos.y <= rect.y + rect.height)
                    {
                        selectedPlatformIds.push_back(obj.id);
                        Console::log("selected platform [id={}]: x={} y={} w={} h={}\n",
                                     obj.id, rect.x, rect.y, rect.width, rect.height);
                        break;
                    }
                }
            }
            else
            {
                selectedPlatformIds.clear();
                Console::log("area selected:\n");
                for (size_t i = 0; i < data.levelData.objectsStatic.size(); ++i)
                {
                    const auto &obj = data.levelData.objectsStatic[i];
                    const lwp::Rectangle &rect = obj.getColliderRect();
                    if (rect.x + rect.width >= selectRect.x && rect.x <= selectRect.x + selectRect.width &&
                        rect.y + rect.height >= selectRect.y && rect.y <= selectRect.y + selectRect.height)
                    {
                        selectedPlatformIds.push_back(obj.id);
                        Console::log("  platform [id={}]: x={} y={} w={} h={}\n",
                                     obj.id, rect.x, rect.y, rect.width, rect.height);
                    }
                }
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            selectedPlatformIds.clear();
        }


    }

    void render(atLevelDesigner_Data &data)
    {
        for (uint32_t id : selectedPlatformIds)
        {
            for (const auto &obj : data.levelData.objectsStatic)
            {
                if (obj.id == id)
                {
                    const lwp::Rectangle &rect_lwp = obj.getColliderRect();
                    Rectangle rect_raylib = atConvert_lwpToRaylib::rectangle(rect_lwp);
                    DrawRectangleLinesEx(rect_raylib, 0.1f, GREEN);
                    break;
                }
            }
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
        selectedPlatformIds.clear();
    }
};
