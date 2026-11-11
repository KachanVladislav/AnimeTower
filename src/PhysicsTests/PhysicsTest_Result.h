#pragma once
#include <vector>
#include <span>
#include <format>
#include "../InputSystem.h"
#include "../lwp/lwp.h"

struct PhysicsTest_PhysicsData
{
    std::vector<InputSystemResult> inputs{};
    std::vector<lwp::RigidBody2D> playerOverFrames{};
    std::vector<lwp::Rectangle> staticPlatforms{};
    int fps = 120;
};

struct PhysicsTest_MessageData
{
    std::string text;
    enum class Type
    {
        fail_assert,
        text
    } type = Type::text;
    static PhysicsTest_MessageData buildText(std::string_view text)
    {
        return PhysicsTest_MessageData{std::string(text), Type::text};
    }
    static PhysicsTest_MessageData buildFail(std::string_view text)
    {
        return PhysicsTest_MessageData{std::string(text), Type::fail_assert};
    }
};

struct PhysicsTest_Result
{
    PhysicsTest_PhysicsData physicsData;
    std::vector<PhysicsTest_MessageData> messages;
    bool isPassed = true;
    void log(std::string_view text)
    {
        messages.push_back(PhysicsTest_MessageData::buildText(text));
    }
    void assert(bool isTrue, std::string_view text)
    {
        if (isTrue)
            return;
        isPassed = false;
        messages.push_back(PhysicsTest_MessageData::buildFail(text));
    }
};