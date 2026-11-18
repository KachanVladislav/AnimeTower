#include "PhysicsTest.h"
#include "atPhysics.h"

PHYSICS_TEST(collisionLeft)
{
    PhysicsTest_Result result;

    PhysicsTest_PhysicsData &physicsData = result.physicsData;
    physicsData.fps = 120;

    atObjectUniqueIdBuilder objBuilder;
    physicsData.staticPlatforms = {
        objBuilder.fromColliderRect({-3.f, 0.0f - 0.5f, 6.f, 0.5f}),
        objBuilder.fromColliderRect({-1.f, 1.4f - 0.5f, 2.f, 0.5f}),
    };

    atObject player = objBuilder.fromColliderRect({0.f, 0.2f, 1.f, 2.f});

    float dT = 1 / (float)physicsData.fps;
    InputSystemResult input;
    input.direction = InputSystemDirection::left;
    for (float time = 0; time < 1; time += dT)
    {
        atPhysics::applyInputToRigidBody(player, input);
        atPhysics::updatePhysics({&player, 1}, physicsData.staticPlatforms, dT);

        physicsData.inputs.push_back(input);
        physicsData.playerOverFrames.push_back(player);
    }

    bool isPlayerCollided = player.getColliderRect().getCenter().x > -1.f;
    result.assert(isPlayerCollided, "Player went through collision");

    return result;
}

PHYSICS_TEST(fallOnFloor)
{
    PhysicsTest_Result result;

    PhysicsTest_PhysicsData &physicsData = result.physicsData;
    physicsData.fps = 120;

    atObjectUniqueIdBuilder objBuilder;
    physicsData.staticPlatforms = {
        objBuilder.fromColliderRect({-1.f, 0.0f - 0.5f, 4.f, 0.5f}),
    };

    atObject player = objBuilder.fromColliderRect({0.f, 0.2f, 1.f, 2.f});

    float dT = 1 / (float)physicsData.fps;
    for (float time = 0; time < 1; time += dT)
    {
        InputSystemResult input;
        atPhysics::applyInputToRigidBody(player, input);
        atPhysics::updatePhysics({&player, 1}, physicsData.staticPlatforms, dT);

        physicsData.inputs.push_back(input);
        physicsData.playerOverFrames.push_back(player);
    }

    bool isPlayerAboveFloor = player.getColliderRect().getCenter().y > 0;
    result.assert(isPlayerAboveFloor, "Player fell under the floor");

    return result;
}

PHYSICS_TEST(jumpTricky) // here to check test vizualizer and to remeber about jump idea
{
    PhysicsTest_Result result;

    PhysicsTest_PhysicsData &physicsData = result.physicsData;
    physicsData.fps = 120;

    atObjectUniqueIdBuilder objBuilder;
    physicsData.staticPlatforms = {
        objBuilder.fromColliderRect({-5.f, 0.0f - 0.5f, 10.f, 0.5f}),
        objBuilder.fromColliderRect({1.2f, 2.1f - 0.5f, 3.2f, 0.5f}),
    };

    atObject player = objBuilder.fromColliderRect({0.f, 0.2f, 1.f, 2.f});

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
        atPhysics::updatePhysics({&player, 1}, physicsData.staticPlatforms, dT);

        physicsData.inputs.push_back(input);
        physicsData.playerOverFrames.push_back(player);

        frameCounter++;
    }

    bool isPlayerPassedPlatform = player.getColliderRect().getCenter().x > 1;
    result.assert(isPlayerPassedPlatform, "Player collided with vertical part after already passed it");

    return result;
}
