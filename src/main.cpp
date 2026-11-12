#include "InputSystem.h"
#include "Console.h"
#include "atPhysics.h"
#include "atCamera.h"
#include "hud/hud.h"
#include "Renderer_lwpToRaylib.h"
#include "atPlatformBuilder.h"
#include "atTextureLoader.h"
#include "atPlatformRenderAtlasLoader.h"
#include "atDefaults.h"

struct atTextures
{
    atPlatformRender::Atlas platformAtlas;
    Texture2D playerTexture;
    void load(atTextureLoader &textureLoader)
    {
        platformAtlas = atPlatformRenderAtlasLoader::load_1_3(textureLoader.loadTextureFromFile("./resource/tile_1_3_64.png"));
        playerTexture = textureLoader.loadTextureFromFile("./resource/elBuba.png");
    }
    void unload()
    {
        UnloadTexture(playerTexture);
        UnloadTexture(platformAtlas.texture);
    }
};

int main()
{
    SetTraceLogLevel(LOG_WARNING); // raylib  logs

    atPlatformBuilder platformBuilder(atDefaults::platformHeight_lwp);
    lwp::Rectangle horizontalPlatforms_lwp[] = {
        // lwp::Rectangle::build_BottomCenter({100.f, 300.f}, {50.f, 100.f}),
        // lwp::Rectangle::build_BottomCenter({500.f, 300.f}, {50.f, 100.f}),
        platformBuilder.buildHorizontalLeft({0.f, 2.7f}, 10.f),
        platformBuilder.buildHorizontalRight({5.f, 2.7f}, 10.f),
        platformBuilder.buildHorizontalRight({7.f, 5.4f}, 3.f),
        platformBuilder.buildHorizontalRight({-2.f, 1.5f}, 20.f)
    };
    static const int horizontalPlatformQty = sizeof(horizontalPlatforms_lwp) / sizeof(horizontalPlatforms_lwp[0]);
    Rectangle horizontalPlatforms_raylib[horizontalPlatformQty];
    for (int i = 0; i < horizontalPlatformQty; i++)
        horizontalPlatforms_raylib[i] = Renderer_lwpToRaylib::lwpToRaylib_rectangle(horizontalPlatforms_lwp[i]);

    lwp::RigidBody2D player;
    const lwp::Vector2f playerDefaultPos{1.1f, 2.f};
    player.box = lwp::Rectangle::build_BottomCenter(playerDefaultPos, {atDefaults::playerW_lwp, atDefaults::playerH_lwp});
    bool isPlayerCurrentFaceRight = true;

    InitWindow(atDefaults::screenWidth, atDefaults::screenHeight, "AnimeTower");
    SetTargetFPS(120);

    InputSystem inputSystem;
    atTextureLoader textureLoader;
    atTextures textures;
    textures.load(textureLoader);
    atCamera camera;
    camera.initialize(atDefaults::screenWidth, atDefaults::screenHeight, atDefaults::screenWidth_lwp);
    HUD hud;
    hud.initialize();

    while (!WindowShouldClose())
    {
        float dT = GetFrameTime(); // if less than cap than cap?
        InputSystemResult input = inputSystem.update();
        if (input.direction)
            isPlayerCurrentFaceRight = (*input.direction == InputSystemDirection::right);
        atPhysics::applyInputToRigidBody(player, input);
        atPhysics::updatePhysics(player, horizontalPlatforms_lwp, dT);

        BeginDrawing();

        camera.setTarget_lwp(player.box.getCenter());
        BeginMode2D(camera.camera);
        ClearBackground(LIGHTGRAY);
        atPlatformRender::drawSpan(textures.platformAtlas, horizontalPlatforms_raylib);
        // Renderer_lwpToRaylib::drawRectangles({&player.box, 1}, PINK);
        Renderer_lwpToRaylib::drawTexture_sameHigh(player.box, textures.playerTexture, isPlayerCurrentFaceRight);
        EndMode2D();

        hud.draw(input);
        EndDrawing();
    }

    textures.unload();
    CloseWindow();

    return 0;
}
