#pragma once
#include "atLevelDesignerMode_Interface.h"
#include "../Console.h"

// #include <algorithm>
// #include <memory>

struct atLevelDesignerSelection
{
    std::vector<atObjectRef> selectedObjectsRef;
    bool isAreaSelecting = false;
    lwp::Vector2f areaSelectStart;
    lwp::Vector2f areaSelectEnd;

    void update(atLevelDesigner_Data &data)
    {
        Vector2 screenMousePos = GetMousePosition();
        bool inViewport = data.screenLayout.isPointInViewport(screenMousePos);

        // Отпускание мыши обрабатываем всегда (даже вне viewport)
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && isAreaSelecting)
        {
            isAreaSelecting = false;
            lwp::Rectangle selectRect = buildRectangleByPos_lwp(areaSelectStart, areaSelectEnd);
            if (selectRect.width < 0.01f && selectRect.height < 0.01f)
            {
                selectedObjectsRef.clear();

                for (auto &collection : data.levelData.currentLayer().collections())
                {
                    std::vector<atObject> &colObj = collection->vec;
                    for (size_t i = 0; i < colObj.size(); ++i)
                    {
                        const auto &obj = colObj[i];
                        const lwp::Rectangle &rect = obj.getColliderRect();
                        lwp::Vector2f mousePos = areaSelectStart;
                        if (mousePos.x >= rect.x && mousePos.x <= rect.x + rect.width &&
                            mousePos.y >= rect.y && mousePos.y <= rect.y + rect.height)
                        {
                            selectedObjectsRef.push_back(atObjectRef(&data.levelData.layers, obj.id));
                            Console::log("selected [id={}]: x={} y={} w={} h={}\n",
                                         obj.id, rect.x, rect.y, rect.width, rect.height);
                            break;
                        }
                    }
                }
            }
            else
            {
                selectedObjectsRef.clear();
                Console::log("area selected:\n");
                for (auto &collection : data.levelData.currentLayer().collections())
                {
                    std::vector<atObject> &colObj = collection->vec;
                    for (size_t i = 0; i < colObj.size(); ++i)
                    {
                        const auto &obj = colObj[i];
                        const lwp::Rectangle &rect = obj.getColliderRect();
                        if (rect.x + rect.width >= selectRect.x && rect.x <= selectRect.x + selectRect.width &&
                            rect.y + rect.height >= selectRect.y && rect.y <= selectRect.y + selectRect.height)
                        {
                            // selectedPlatformIds.push_back(obj.id);
                            selectedObjectsRef.push_back(atObjectRef(&data.levelData.layers, obj.id));
                            Console::log("  platform [id={}]: x={} y={} w={} h={}\n",
                                         obj.id, rect.x, rect.y, rect.width, rect.height);
                        }
                    }
                }
            }
            return;
        }

        if (!inViewport)
            return;

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
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            selectedObjectsRef.clear();
        }
    }

    void render(atLevelDesigner_Data &data)
    {
        Rectangle vp = data.screenLayout.viewport;
        BeginScissorMode((int)vp.x, (int)vp.y, (int)vp.width, (int)vp.height);

        for (auto &objRef : selectedObjectsRef)
        {
            atObject *obj = objRef.resolve();
            if (obj)
            {
                const lwp::Rectangle &rect_lwp = obj->getColliderRect();
                Rectangle rect_raylib = atConvert_lwpToRaylib::rectangle(rect_lwp);
                DrawRectangleLinesEx(rect_raylib, 0.1f, GREEN);
            }
        }

        if (isAreaSelecting)
        {
            lwp::Rectangle selectRect_lwp = buildRectangleByPos_lwp(areaSelectStart, areaSelectEnd);
            Rectangle selectRect_raylib = atConvert_lwpToRaylib::rectangle(selectRect_lwp);
            DrawRectangleLinesEx(selectRect_raylib, 0.05f, BLUE);
        }

        EndScissorMode();
    }

    void clear()
    {
        selectedObjectsRef.clear();
    }
};
