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

int main()
{
    SetTraceLogLevel(LOG_WARNING); // raylib  logs

    // atPlatformBuilder platformBuilder(atDefaults::platformHeight_lwp);
    // atLevelData levelData;
    // levelData.horizontalPlatforms_lwp = std::vector<lwp::Rectangle>{
    //     platformBuilder.buildHorizontalLeft({0.f, 2.7f}, 10.f),
    //     platformBuilder.buildHorizontalRight({5.f, 2.7f}, 10.f),
    //     platformBuilder.buildHorizontalRight({7.f, 5.4f}, 3.f),
    //     platformBuilder.buildHorizontalRight({-2.f, 1.5f}, 20.f)};
    // levelData.objectsRects_lwp = std::vector<lwp::Rectangle>{
    //     lwp::Rectangle::build_BottomRight({0.f, 2.7f}, {1.f, 2.f}),
    // };
    // levelData.playerPos = lwp::Vector2f{1.1f, 2.f};
    // atLevelDataLoader::saveLevelSourceData("./resource/levels/defaultLevel.txt",levelData);
    atLevelData levelData;
    atLevelDataLoader::loadLevelSourceData("./resource/levels/defaultLevel.txt", levelData);

    InitWindow(atDefaults::screenWidth, atDefaults::screenHeight, "AnimeTower");
    SetTargetFPS(120);

    InputSystem inputSystem;
    atResourceRender textures;
    textures.load();
    Camera2D camera;
    atCamera::setCameraToScreen_lwp(camera, atDefaults::screenWidth_lwp);
    HUD hud;
    hud.initialize();

    while (!WindowShouldClose())
    {
        float dT = GetFrameTime();
        InputSystemResult input = inputSystem.update();

        atObject *player = levelData.playerRef.resolve();
        if (player)
        {
            atPhysics::applyInputToRigidBody(*player, input);
            if (input.direction)
                player->isFlipX = (*input.direction == InputSystemDirection::right);
            atCamera::setCameraTarget_lwp(camera, player->getColliderRect().getCenter());
        }

        atPhysics::updatePhysics(levelData.objectsRigid, levelData.objectsStatic, dT);

        BeginDrawing();

        BeginMode2D(camera);
        atRenderLevel::render(levelData, textures);

        EndMode2D();

        hud.draw(input);
        EndDrawing();
    }

    textures.unload();
    CloseWindow();

    return 0;
}
