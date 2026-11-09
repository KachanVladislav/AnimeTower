#include "InputSystem.h"
#include "Console.h"
#include "atPhysics.h"
#include "PhysicsTests/PhysicsTest_Result.h"
#include "PhysicsTests/PhysicsTest_Visualizer.h"
#include "atPlatformBuilder.h"


PhysicsTest_Result PhysicsTestRun_fallOnFloor()
{
    PhysicsTest_Result result;
    PhysicsTest_VisualizerData &physicsData = result.physicsData;
    physicsData.fps = 120;

    atPlatformBuilder platformBuilder;
    physicsData.staticPlatforms = {
        // lwp::Rectangle::build_BottomCenter({100.f, 300.f}, {50.f, 100.f}),
        // lwp::Rectangle::build_BottomCenter({500.f, 300.f}, {50.f, 100.f}),
        platformBuilder.buildHorizontalLeft({0.f, 2.7f}, 10.f),
        platformBuilder.buildHorizontalRight({5.f, 2.7f}, 10.f),
        platformBuilder.buildHorizontalRight({7.f, 5.4f}, 3.f),
        platformBuilder.buildHorizontalRight({-2.f, 1.5f}, 20.f)};

    lwp::RigidBody2D player;
    player.box = lwp::Rectangle::build_BottomCenter({1.1f, 2.f}, {1.f, 2.f});

    float dT = 1 / (float)physicsData.fps;
    for (float time = 0; time < 3; time += dT)
    {
        InputSystemResult input;
        atPhysics::applyInputToRigidBody(player, input);
        atPhysics::updatePhysics(player, physicsData.staticPlatforms, dT);

        physicsData.inputs.push_back(input);
        physicsData.playerOverFrames.push_back(player);
    }

    return result;
}

int main()
{
    PhysicsTest_Result testResult = PhysicsTestRun_fallOnFloor();

    PhysicsTest_Visualizer vizualizer;
    vizualizer.initialize();
    vizualizer.vizualize(testResult.physicsData.buildView());
    return 0;
}
