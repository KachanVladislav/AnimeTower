#pragma once
#include "raylib.h"
#include <fstream>
#include <string>

struct atLevelDesignerScreenLayout
{
    Rectangle viewport;
    Rectangle inspector;
    Rectangle console;
    Rectangle menuBar;
    Rectangle scene;

    float menuBarHeight = 100.f;
    float inspectorWidth = 280.f;
    float consoleHeight = 180.f;
    float sceneWidth = 200.f;

    static inline const char *CONFIG_PATH = "./resource/levels/screenLayout.ini";

    int windowSize_x{};//just for a moment here:)
    int windowSize_y{};

    void loadConfig()
    {
        std::ifstream file(CONFIG_PATH);
        if (!file.is_open()) return;

        std::string line;
        windowSize_x = 640;
        windowSize_y = 480;
        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#') continue;
            auto eq = line.find('=');
            if (eq == std::string::npos) continue;
            std::string key = line.substr(0, eq);
            float value = std::stof(line.substr(eq + 1));
            if (key == "menuBarHeight") menuBarHeight = value;
            else if (key == "inspectorWidth") inspectorWidth = value;
            else if (key == "consoleHeight") consoleHeight = value;
            else if (key == "sceneWidth") sceneWidth = value;
            else if (key == "winSize_x") windowSize_x = (int)value;
            else if (key == "winSize_y") windowSize_y = (int)value;
        }
    }

    void saveConfig()
    {
        std::ofstream file(CONFIG_PATH);
        if (!file.is_open()) return;
        file << "# ScreenLayout config\n";
        file << "menuBarHeight=" << menuBarHeight << "\n";
        file << "inspectorWidth=" << inspectorWidth << "\n";
        file << "consoleHeight=" << consoleHeight << "\n";
        file << "sceneWidth=" << sceneWidth << "\n";
        file << "winSize_x=" << windowSize_x << "\n";
        file << "winSize_y=" << windowSize_y << "\n";
    }

    // Drag state
    bool isDragging = false;
    Vector2 startDragPos;
    float startValue;
    enum class DragTarget { None, MenuBar, Inspector, Console, Scene };
    DragTarget dragTarget = DragTarget::None;

    void update(float screenWidth, float screenHeight)
    {
        menuBar = {0, 0, screenWidth, menuBarHeight};
        scene = {0, menuBarHeight, sceneWidth, screenHeight - menuBarHeight - consoleHeight};
        viewport = {sceneWidth, menuBarHeight, screenWidth - sceneWidth - inspectorWidth, screenHeight - menuBarHeight - consoleHeight};
        inspector = {screenWidth - inspectorWidth, menuBarHeight, inspectorWidth, screenHeight - menuBarHeight};
        console = {0, screenHeight - consoleHeight, screenWidth - inspectorWidth, consoleHeight};
    }

    void beforeRender(float screenWidth, float screenHeight)
    {
        Vector2 mousePos = GetMousePosition();
        
        if (isDragging)
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                switch (dragTarget)
                {
                case DragTarget::MenuBar:
                    menuBarHeight = startValue + (mousePos.y - startDragPos.y);
                    if (menuBarHeight < 20) menuBarHeight = 20;
                    else if (menuBarHeight > screenHeight / 2) menuBarHeight = screenHeight / 2;
                    break;
                case DragTarget::Inspector:
                    inspectorWidth = startValue - (mousePos.x - startDragPos.x);
                    if (inspectorWidth < 150) inspectorWidth = 150;
                    else if (inspectorWidth > screenWidth / 2) inspectorWidth = screenWidth / 2;
                    break;
                case DragTarget::Console:
                    consoleHeight = startValue - (mousePos.y - startDragPos.y);
                    if (consoleHeight < 50) consoleHeight = 50;
                    else if (consoleHeight > screenHeight / 2) consoleHeight = screenHeight / 2;
                    break;
                case DragTarget::Scene:
                    sceneWidth = startValue + (mousePos.x - startDragPos.x);
                    if (sceneWidth < 100) sceneWidth = 100;
                    else if (sceneWidth > screenWidth / 3) sceneWidth = screenWidth / 3;
                    break;
                }
            }
            else
            {
                isDragging = false;
                dragTarget = DragTarget::None;
            }
        }
        else
        {
            // MenuBar edge (bottom)
            float menuDelta = menuBarHeight - mousePos.y;
            bool inMenuArea = menuDelta > 0 && menuDelta < 5 && mousePos.x < screenWidth - inspectorWidth;
            
            // Inspector edge (left)
            float inspectorEdgeX = screenWidth - inspectorWidth;
            float inspectorDelta = mousePos.x - inspectorEdgeX;
            bool inInspectorArea = inspectorDelta > 0 && inspectorDelta < 5 && mousePos.y > menuBarHeight;
            
            // Console edge (top)
            float consoleEdgeY = screenHeight - consoleHeight;
            float consoleDelta = mousePos.y - consoleEdgeY;
            bool inConsoleArea = consoleDelta > 0 && consoleDelta < 5 && mousePos.x < screenWidth - inspectorWidth;

            // Scene edge (right)
            float sceneEdgeX = sceneWidth;
            float sceneDelta = mousePos.x - sceneEdgeX;
            bool inSceneArea = sceneDelta > 0 && sceneDelta < 5 && mousePos.y > menuBarHeight && mousePos.y < screenHeight - consoleHeight;

            if (inMenuArea || inInspectorArea || inConsoleArea || inSceneArea)
            {
                if (inMenuArea)
                    SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
                else if (inInspectorArea)
                    SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
                else if (inSceneArea)
                    SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
                else
                    SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    isDragging = true;
                    startDragPos = mousePos;
                    if (inMenuArea) { dragTarget = DragTarget::MenuBar; startValue = menuBarHeight; }
                    else if (inInspectorArea) { dragTarget = DragTarget::Inspector; startValue = inspectorWidth; }
                    else if (inSceneArea) { dragTarget = DragTarget::Scene; startValue = sceneWidth; }
                    else { dragTarget = DragTarget::Console; startValue = consoleHeight; }
                }
            }
            else
            {
                SetMouseCursor(MOUSE_CURSOR_ARROW);
            }
        }
    }

    bool isPointInViewport(Vector2 point) const
    {
        return CheckCollisionPointRec(point, viewport);
    }

    bool isPointInInspector(Vector2 point) const
    {
        return CheckCollisionPointRec(point, inspector);
    }

    bool isPointInConsole(Vector2 point) const
    {
        return CheckCollisionPointRec(point, console);
    }

    bool isPointInMenuBar(Vector2 point) const
    {
        return CheckCollisionPointRec(point, menuBar);
    }

    bool isPointInScene(Vector2 point) const
    {
        return CheckCollisionPointRec(point, scene);
    }

    void drawDebug() const
    {
        DrawRectangleLinesEx(viewport, 1.f, RED);
        DrawRectangleLinesEx(inspector, 1.f, GREEN);
        DrawRectangleLinesEx(console, 1.f, BLUE);
        DrawRectangleLinesEx(menuBar, 1.f, YELLOW);
        DrawRectangleLinesEx(scene, 1.f, PURPLE);
    }
};
