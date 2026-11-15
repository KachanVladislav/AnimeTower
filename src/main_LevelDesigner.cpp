#include "atLevelDesigner/atLevelDesignerMode_Hub.h"
#include "atLevelDesigner/atLevelDesignerMode_PlayTest.h"
#include "atLevelDesigner/atLevelDesignerMode_PlacePlatform.h"

#include "Console.h"
#include "atCamera.h"
#include "atResource/atResourceRender.h"
#include "atRender/atRenderLevel.h"

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
    
    void initialize()
    {
        data.levelData = atLevelDataLoader::loadLevelSourceData(LEVEL_PATH);
        data.player.rect = lwp::Rectangle::build_BottomCenter(data.levelData.playerPos, {atDefaults::playerW_lwp, atDefaults::playerH_lwp});
        data.updateRenderData();
    }
    
    void processModeChange();
    atLevelDesignerModeInterface *getModeById(atLevelDesignerModeId modeId);
};

int main()
{
    atLevelDesigner levelDesigner;
    levelDesigner.initialize();
    SetTraceLogLevel(LOG_WARNING); // raylib  logs

    InitWindow(atDefaults::screenWidth, atDefaults::screenHeight, "atLevelDesigner");
    SetTargetFPS(120);

    atResourceRender textures;
    textures.load();
    atCamera camera;
    camera.initialize(atDefaults::screenWidth, atDefaults::screenHeight, atDefaults::screenWidth_lwp);

    while (!WindowShouldClose())
    {
        levelDesigner.processModeChange();

        levelDesigner.currentMode->beforeRender(levelDesigner.data);

        BeginDrawing();

        camera.setTarget_lwp(levelDesigner.data.player.rect.getCenter());
        levelDesigner.data.camera = camera.camera;

        BeginMode2D(camera.camera);
        atRenderLevel::render(levelDesigner.data.renderData, textures);
        levelDesigner.currentMode->render_2D(levelDesigner.data);
        EndMode2D();
        levelDesigner.currentMode->render_Screen(levelDesigner.data);

        lwp::Vector2f mousePos_lwp = getMousePos_lwp(camera.camera);
        auto mousePos_str = std::format("{:.1f} {:.1f}", mousePos_lwp.x, mousePos_lwp.y);
        GuiLabel({10, GetScreenHeight() - 10.f, 200, 10}, mousePos_str.c_str());

        EndDrawing();
        levelDesigner.currentMode->afterRender(levelDesigner.data);
    }

    textures.unload();
    CloseWindow();

    return 0;
}

atLevelDesignerModeInterface* atLevelDesigner::getModeById(atLevelDesignerModeId modeId)
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