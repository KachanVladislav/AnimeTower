#include "InputSystem.h"
#include "Console.h"
#include "atCamera.h"
#include "hud/hud.h"

#include "atLevel/atLevelData.h"
#include "atPhysics.h"
#include "atConvert_lwpToRaylib.h"
#include "atResource/atResourceRender.h"
#include "atRender/atRenderLevel.h"
#include "atDefaults.h"
#include "raygui.h"

const char* LEVEL_PATH = "./resource/levels/defaultLevel.txt";
const char* LEVEL_PATH_TO_SAVE = "./resource/levels/levelFromDesigner.txt";

int main()
{
    SetTraceLogLevel(LOG_WARNING); // raylib  logs

    atLevelData levelData = atLevelDataLoader::loadLevelSourceData(LEVEL_PATH);
    lwp::RigidBody2D player;
    player.rect = lwp::Rectangle::build_BottomCenter(levelData.playerPos, {atDefaults::playerW_lwp, atDefaults::playerH_lwp});

    atRenderLevelData renderData;
    renderData.horizontalPlatforms_raylib = atConvert_lwpToRaylib::buildRectsVector(levelData.horizontalPlatforms_lwp);
    renderData.objectsRects_raylib = atConvert_lwpToRaylib::buildRectsVector(levelData.objectsRects_lwp);
    renderData.playerRect = atConvert_lwpToRaylib::rectangle(player.rect);

    InitWindow(atDefaults::screenWidth, atDefaults::screenHeight, "AnimeTower");
    SetTargetFPS(120);

    InputSystem inputSystem;
    atResourceRender textures;
    textures.load();
    atCamera camera;
    camera.initialize(atDefaults::screenWidth, atDefaults::screenHeight, atDefaults::screenWidth_lwp);
    HUD hud;
    hud.initialize();

    while (!WindowShouldClose())
    {
        // float dT = GetFrameTime(); // if less than cap than cap?
        InputSystemResult input = inputSystem.update();
        // atPhysics::applyInputToRigidBody(player, input);
        // atPhysics::updatePhysics(player, levelData.horizontalPlatforms_lwp, dT);

        renderData.playerRect = atConvert_lwpToRaylib::rectangle(player.rect);
        if (input.direction)
            renderData.isPlayerFaceRight = (*input.direction == InputSystemDirection::right);

        BeginDrawing();

        camera.setTarget_lwp(player.rect.getCenter());
        BeginMode2D(camera.camera);
        atRenderLevel::render(renderData, textures);
        EndMode2D();

        if (GuiButton({atDefaults::screenWidth - 80 - 10, 10, 80, 30}, "Save"))
        {
            atLevelDataLoader::saveLevelSourceData(LEVEL_PATH_TO_SAVE, levelData);
        }

        hud.draw(input);
        EndDrawing();
    }

    textures.unload();
    CloseWindow();

    return 0;
}
