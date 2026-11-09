#pragma once
#include "PhysicsTest_Visualizer.h"
#include "../hud/hud.h"
#include "../atCamera.h"
#include "../Renderer_lwpToRaylib.h"

class PhysicsTest_Visualizer
{
    HUD hud;
    atCamera camera;
    struct TestVisualizerSettings
    {
        int screenWidth = 800;
        int screenHeight = 450;
        float screenWidth_lwp = 20;
    } settings;

public:
    void vizualize(const PhysicsTest_PhysicsData &data)
    {
        if (isVizualizedNotValidData(data))
        {
            return;
        }
        
        camera.initialize(settings.screenWidth, settings.screenHeight, settings.screenWidth_lwp);
        InitWindow(settings.screenWidth, settings.screenHeight, "testAnimeTower");
        hud.initialize();
        SetTargetFPS(data.fps);

        size_t currentIndex = 0;
        size_t maxIndex = data.inputs.size();
        const float dT = 1 / (float)data.fps;
        const float maxTime = maxIndex * dT;

        while (!WindowShouldClose())
        {
            InputSystemResult input = data.inputs[currentIndex];
            const lwp::RigidBody2D &player = data.playerOverFrames[currentIndex];

            camera.setTarget_lwp(player.box.getCenter());

            BeginDrawing();

            BeginMode2D(camera.camera);
            ClearBackground(RAYWHITE);
            Renderer_lwpToRaylib::drawRectangles(data.staticPlatforms, GREEN);
            Renderer_lwpToRaylib::drawRectangles({&player.box, 1}, PINK);
            EndMode2D();

            hud.draw(input);
            drawTime(currentIndex * dT, maxTime);

            EndDrawing();

            currentIndex++;
            if (currentIndex >= maxIndex)
                currentIndex = 0;
        }
        CloseWindow();
    }

private:
    bool isVizualizedNotValidData(const PhysicsTest_PhysicsData &data)
    {
        if (data.inputs.size() != data.playerOverFrames.size() || data.inputs.size() == 0)
        {
            std::string errorMessage = std::format("Wrong size {}/{}",
                                                   data.inputs.size(),
                                                   data.playerOverFrames.size());
            InitWindow(settings.screenWidth, settings.screenHeight, "testAnimeTower_NotValidArgs");
            while (!WindowShouldClose())
            {
                BeginDrawing();
                DrawText(errorMessage.c_str(), 0, settings.screenHeight / 2, 20, RED);
                EndDrawing();
            }
            CloseWindow();
            return true;
        }
        return false;
    }

    void drawTime(float currentTime, float maxTime)
    {
        DrawText(std::format(" {:.1f}/{:.1f}s", currentTime, maxTime).c_str(), 0, settings.screenHeight - 20, 20, RED);
    }
};