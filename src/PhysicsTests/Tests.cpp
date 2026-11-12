#include "PhysicsTest.h"
#include "../atPlatformBuilder.h"
#include "../atPhysics.h"

PHYSICS_TEST(collisionLeft)
{
    PhysicsTest_Result result;

    PhysicsTest_PhysicsData &physicsData = result.physicsData;
    physicsData.fps = 120;

    atPlatformBuilder platformBuilder(0.5f);
    physicsData.staticPlatforms = {
        platformBuilder.buildHorizontalRight({-3.f, 0.0f}, 6.f),
        platformBuilder.buildHorizontalLeft({-1.f, 1.4f}, 2.f),
    };

    lwp::RigidBody2D player;
    player.box = lwp::Rectangle::build_BottomCenter({0.f, 0.f}, {1.f, 2.f});

    float dT = 1 / (float)physicsData.fps;
    InputSystemResult input;
    input.direction = InputSystemDirection::left;
    for (float time = 0; time < 1; time += dT)
    {
        atPhysics::applyInputToRigidBody(player, input);
        atPhysics::updatePhysics(player, physicsData.staticPlatforms, dT);

        physicsData.inputs.push_back(input);
        physicsData.playerOverFrames.push_back(player);
    }

    bool isPlayerCollided = player.box.getCenter().x > -1.f;
    result.assert(isPlayerCollided, "Player went through collision");

    return result;
}

PHYSICS_TEST(fallOnFloor)
{
    PhysicsTest_Result result;
    PhysicsTest_PhysicsData &physicsData = result.physicsData;
    physicsData.fps = 120;

    atPlatformBuilder platformBuilder(0.5f);
    physicsData.staticPlatforms = {
        platformBuilder.buildHorizontalRight({0.f, 0.0f}, 2.f),
    };

    lwp::RigidBody2D player;
    player.box = lwp::Rectangle::build_BottomCenter({0.f, 0.f}, {1.f, 2.f});

    float dT = 1 / (float)physicsData.fps;
    for (float time = 0; time < 1; time += dT)
    {
        InputSystemResult input;
        atPhysics::applyInputToRigidBody(player, input);
        atPhysics::updatePhysics(player, physicsData.staticPlatforms, dT);

        physicsData.inputs.push_back(input);
        physicsData.playerOverFrames.push_back(player);
    }

    bool isPlayerAboveFloor = player.box.getCenter().y > 0;
    result.assert(isPlayerAboveFloor, "Player fell under the floor");

    return result;
}

PHYSICS_TEST(jumpTricky)
{
    PhysicsTest_Result result;
    PhysicsTest_PhysicsData &physicsData = result.physicsData;
    physicsData.fps = 120;

    atPlatformBuilder platformBuilder(0.5f);
    physicsData.staticPlatforms = {
        platformBuilder.buildHorizontalRight({-5.f, 0.0f}, 10.f),
        platformBuilder.buildHorizontalRight({-0.2f, 2.1f}, 3.2f),
    };
    lwp::RigidBody2D player;
    player.box = lwp::Rectangle::build_BottomCenter({0.f, 0.f}, {1.f, 2.f});

    auto getInput = [](int frameCounter) -> InputSystemResult
    {
        if (frameCounter < 20)
            return {};
        InputSystemResult input;
        input.direction = InputSystemDirection::right;
        if (frameCounter > 21)
            input.action = InputSystemAction::jump;
        return input;
    };

    float dT = 1 / (float)physicsData.fps;
    int frameCounter = 0;
    for (float time = 0; time < 1; time += dT)
    {
        InputSystemResult input = getInput(frameCounter);
        atPhysics::applyInputToRigidBody(player, input);
        atPhysics::updatePhysics(player, physicsData.staticPlatforms, dT);

        physicsData.inputs.push_back(input);
        physicsData.playerOverFrames.push_back(player);

        frameCounter++;
    }

    bool isPlayerPassedPlatform = player.box.getCenter().x > 1;
    result.assert(isPlayerPassedPlatform, "Player collided with vertical part after already passed it");

    return result;
}