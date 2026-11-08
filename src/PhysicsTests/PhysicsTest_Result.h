#pragma once
#include <vector>
#include <span>
#include <format>
#include "../InputSystem.h"
#include "../lwp/lwp.h"

struct PhysicsTest_VisualizerData_View
{
    int fps = 120;
    std::span<const InputSystemResult> inputs{};
    std::span<const lwp::Rectangle> staticPlatforms{};
    std::span<const lwp::RigidBody2D> playerOverFrames{};
};

struct PhysicsTest_VisualizerData
{
    std::vector<InputSystemResult> inputs{};
    std::vector<lwp::RigidBody2D> playerOverFrames{};
    std::vector<lwp::Rectangle> staticPlatforms{};
    int fps = 120;
    PhysicsTest_VisualizerData_View buildView()
    {
        PhysicsTest_VisualizerData_View res;
        res.fps = fps;
        res.inputs = inputs;
        res.staticPlatforms = staticPlatforms;
        res.playerOverFrames = playerOverFrames;
        return res;
    }
};

struct PhysicsTest_Result
{
    PhysicsTest_VisualizerData physicsData;
    bool isPassed = true;
    struct MessageData
    {
        size_t idx = 0;
        std::string message;
        enum class Type
        {
            fail,
            text
        } type = Type::text;
        static MessageData buildText(size_t idx, std::string_view text)
        {
            return MessageData{idx, std::string(text), Type::text};
        }
    };
    std::vector<MessageData> messages;
    void log(size_t idx, std::string_view text)
    {
        messages.push_back(MessageData::buildText(idx, text));
    }
};