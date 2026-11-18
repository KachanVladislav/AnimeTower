#include "atLevelDesigner/atLevelDesignerMode_Hub.h"
#include "atLevelDesigner/atLevelDesignerMode_PlayTest.h"
#include "atLevelDesigner/atLevelDesignerMode_PlacePlatform.h"

#include "Console.h"
#include "atCamera.h"
#include "atResource/atResourceRender.h"
#include "atRender/atRenderLevel.h"
#include "atCameraZoomPanControl.h"

const char *LEVEL_PATH = "./resource/levels/levelFromDesigner.txt";
struct atLevelDesigner
{
    atLevelDesignerMode_Hub modeHub;
    atLevelDesignerMode_PlayTest modePlayTest;
    atLevelDesignerMode_PlacePlatform modePlacePlatform;
    enum class atLevelDesignerModeId
    {
        Hub,
        PlayTest,
        PlacePlatform,
    };
    atLevelDesignerModeInterface *currentMode = &modeHub;
    atLevelDesignerModeId currentModeId = atLevelDesignerModeId::Hub;

    atLevelDesigner_Data data;
    atLevelDesignerOp_Executer opExecuter;
    void initialize()
    {
        data.screenLayout.loadConfig();
        atLevelDataLoader::loadLevelSourceData(LEVEL_PATH, data.levelData);
    }

    void processModeChange();
    atLevelDesignerModeInterface *getModeById(atLevelDesignerModeId modeId);
};

int main()
{
    atCameraZoomPanControl cameraZoomPanControl;
    atLevelDesigner levelDesigner;
    levelDesigner.initialize();
    SetTraceLogLevel(LOG_WARNING); // raylib  logs

    InitWindow(atDefaults::screenWidth, atDefaults::screenHeight, "atLevelDesigner");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(120);

    atResourceRender textures;
    textures.load();

    atCamera::setCameraToScreen_lwp(levelDesigner.data.camera, atDefaults::screenWidth_lwp);

    atLevelDesigner_Data &data = levelDesigner.data;
    atLevelDesignerOp_Executer &opExecuter = levelDesigner.opExecuter;
    while (!WindowShouldClose())
    {
        levelDesigner.processModeChange();
        levelDesigner.currentMode->beforeRender(data, opExecuter);
        Rectangle vp = data.screenLayout.viewport;
        atObject *player = levelDesigner.data.levelData.playerRef.resolve();
        if (player)
            cameraZoomPanControl.update(data.camera, player->getColliderRect(), vp);
        else
            cameraZoomPanControl.update(data.camera, {}, vp);
            
        data.screenLayout.beforeRender((float)GetScreenWidth(), (float)GetScreenHeight());
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Рендеринг уровня только в viewport

        BeginScissorMode((int)vp.x, (int)vp.y, (int)vp.width, (int)vp.height);
        BeginMode2D(data.camera);
        atRenderLevel::render(data.levelData, textures);
        levelDesigner.currentMode->render_2D(data);
        EndMode2D();
        EndScissorMode();

        // Границы панелей
        DrawRectangleLinesEx(vp, 2.f, DARKGRAY);
        DrawRectangleLinesEx(data.screenLayout.scene, 2.f, DARKGRAY);

        // Screen-space UI
        levelDesigner.currentMode->render_Screen(data);

        lwp::Vector2f mousePos_lwp = getMousePos_lwp(data.camera);
        auto mousePos_str = std::format("{:.1f} {:.1f}", mousePos_lwp.x, mousePos_lwp.y);
        GuiLabel({10, GetScreenHeight() - 10.f, 200, 10}, mousePos_str.c_str());

        EndDrawing();
        levelDesigner.currentMode->afterRender(data, opExecuter);

        if (IsKeyPressed(KEY_Z) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)))
        {
            levelDesigner.opExecuter.undo(data);
        }
        if (IsKeyPressed(KEY_Y) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)))
        {
            levelDesigner.opExecuter.redo(data);
        }
    }

    textures.unload();
    levelDesigner.data.screenLayout.saveConfig();
    CloseWindow();

    return 0;
}

atLevelDesignerModeInterface *atLevelDesigner::getModeById(atLevelDesignerModeId modeId)
{
    if (modeId == atLevelDesignerModeId::Hub)
        return &modeHub;
    else if (modeId == atLevelDesignerModeId::PlayTest)
        return &modePlayTest;
    else if (modeId == atLevelDesignerModeId::PlacePlatform)
        return &modePlacePlatform;
    return &modeHub;
};

void atLevelDesigner::processModeChange()
{
    atLevelDesignerModeId newModeId = currentModeId;
    if (currentModeId == atLevelDesignerModeId::Hub)
    {
        if (IsKeyPressed(KEY_F1))
            newModeId = atLevelDesignerModeId::PlayTest;
        if (IsKeyPressed(KEY_B))
            newModeId = atLevelDesignerModeId::PlacePlatform;
    }
    else if (currentModeId == atLevelDesignerModeId::PlayTest)
    {
        if (IsKeyPressed(KEY_F1))
            newModeId = atLevelDesignerModeId::Hub;
    }
    else if (currentModeId == atLevelDesignerModeId::PlacePlatform)
    {
        if (IsKeyPressed(KEY_B))
            newModeId = atLevelDesignerModeId::Hub;
    }

    if (newModeId != currentModeId)
    {
        currentMode->onExit(data);
        currentMode = getModeById(newModeId);
        currentMode->onStart(data);
        currentModeId = newModeId;
    }
};