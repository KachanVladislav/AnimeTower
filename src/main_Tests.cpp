
#include "PhysicsTests/PhysicsTest.h"
#include "PhysicsTests/PhysicsTest_Visualizer.h"
#include "Console.h"

int main()
{
    SetTraceLogLevel(LOG_WARNING);//raylib  logs
    const bool VISUALIZE_ON_FAIL = true;

    PhysicsTest_Visualizer vizualizer;
    for (const auto &test : PhysicsTest_EntriesRegistrar::getTests())
    {
        PhysicsTest_Result testResult = test.func();
        if(testResult.isPassed)
            Console::log("[TEST PASSED] {}\n", test.name);
        else
            Console::log("[TEST FAIL] {}\n", test.name);
            
        for(const auto& message: testResult.messages)
        {
            if(message.type == PhysicsTest_MessageData::Type::fail_assert)
                Console::log("  [FAIL ASSERT] {}\n", message.text);
            else
                Console::log("  {}\n", message.text);
        }

        if(VISUALIZE_ON_FAIL && !testResult.isPassed)
            vizualizer.vizualize(testResult.physicsData);
    }
    
    return 0;
}

#include "atLevel/atLevelData.h"
#include <string>
#include <cstdio>
PHYSICS_TEST(atLevel_saveLoad)
{
    PhysicsTest_Result result;

    atLevelData original{};
    original.playerPos = {10.5f, 20.3f};
    original.horizontalPlatforms_lwp = {
        {0.f, 0.f, 10.f, 1.f},
        {-5.f, 2.f, 3.f, 0.5f},
    };
    original.objectsRects_lwp = {
        {1.f, 1.f, 2.f, 2.f},
    };

    const char *testFile = "test_level_save_load.txt";
    atLevelDataLoader::saveLevelSourceData(testFile, original);

    atLevelData loaded = atLevelDataLoader::loadLevelSourceData(testFile);

    bool posMatch = loaded.playerPos.x == original.playerPos.x && loaded.playerPos.y == original.playerPos.y;
    result.assert(posMatch, "playerPos mismatch");

    bool platformsCountMatch = loaded.horizontalPlatforms_lwp.size() == original.horizontalPlatforms_lwp.size();
    result.assert(platformsCountMatch, "horizontalPlatforms count mismatch");

    if (platformsCountMatch)
    {
        for (size_t i = 0; i < original.horizontalPlatforms_lwp.size(); i++)
        {
            const auto &orig = original.horizontalPlatforms_lwp[i];
            const auto &load = loaded.horizontalPlatforms_lwp[i];
            bool match = orig.x == load.x && orig.y == load.y && orig.width == load.width && orig.height == load.height;
            result.assert(match, "horizontalPlatforms[" + std::to_string(i) + "] mismatch");
        }
    }

    bool objectsCountMatch = loaded.objectsRects_lwp.size() == original.objectsRects_lwp.size();
    result.assert(objectsCountMatch, "objectsRects count mismatch");

    if (objectsCountMatch)
    {
        for (size_t i = 0; i < original.objectsRects_lwp.size(); i++)
        {
            const auto &orig = original.objectsRects_lwp[i];
            const auto &load = loaded.objectsRects_lwp[i];
            bool match = orig.x == load.x && orig.y == load.y && orig.width == load.width && orig.height == load.height;
            result.assert(match, "objectsRects[" + std::to_string(i) + "] mismatch");
        }
    }

    std::remove(testFile);

    return result;
}
