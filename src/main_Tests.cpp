
#include "PhysicsTests/PhysicsTest.h"
#include "PhysicsTests/PhysicsTest_Visualizer.h"
#include "Console.h"

int main()
{
    SetTraceLogLevel(LOG_WARNING);//raylib  logs
    const bool VISUALIZE_ALL = false;
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

        if(VISUALIZE_ALL)
            vizualizer.vizualize(testResult.physicsData);
        else if(VISUALIZE_ON_FAIL && !testResult.isPassed)
            vizualizer.vizualize(testResult.physicsData);
    }
    
    return 0;
}

#include "atLevel/atLevelData.h"
#include <string>
#include <cstdio>
#include <set>
PHYSICS_TEST(atLevel_saveLoad)
{
    PhysicsTest_Result result;

    atLevelData original{};
    original.playerDefaultPos = {10.5f, 20.3f};
    original.layers.push_back({});
    auto &layer = original.currentLayer();
    layer.colStatic.vec = {
        original.objectBuilder.fromColliderRect({0.f, 0.f, 10.f, 1.f}),
        original.objectBuilder.fromColliderRect({-5.f, 2.f, 3.f, 0.5f}),
    };
    layer.colNotCollidable.vec = {
        original.objectBuilder.fromColliderRect({1.f, 1.f, 2.f, 2.f}),
    };

    const char *testFile = "test_level_save_load.txt";
    atLevelDataLoader::saveLevelSourceData(testFile, original);

    atLevelData loaded = {};
    atLevelDataLoader::loadLevelSourceData(testFile, loaded);

    bool posMatch = loaded.playerDefaultPos.x == original.playerDefaultPos.x && loaded.playerDefaultPos.y == original.playerDefaultPos.y;
    result.assert(posMatch, "playerPos mismatch");

    bool layerCountMatch = loaded.layers.size() == original.layers.size();
    result.assert(layerCountMatch, "layers count mismatch");

    if (layerCountMatch)
    {
        auto &origLayer = original.layers[0];
        auto &loadLayer = loaded.layers[0];

        bool staticCountMatch = loadLayer.colStatic.vec.size() == origLayer.colStatic.vec.size();
        result.assert(staticCountMatch, "colStatic count mismatch");

        if (staticCountMatch)
        {
            for (size_t i = 0; i < origLayer.colStatic.vec.size(); i++)
            {
                const auto &origRect = origLayer.colStatic.vec[i].getColliderRect();
                const auto &loadRect = loadLayer.colStatic.vec[i].getColliderRect();
                bool match = origRect.x == loadRect.x && origRect.y == loadRect.y && origRect.width == loadRect.width && origRect.height == loadRect.height;
                result.assert(match, "colStatic[" + std::to_string(i) + "] mismatch");
            }
        }

        bool notCollidableCountMatch = loadLayer.colNotCollidable.vec.size() == origLayer.colNotCollidable.vec.size();
        result.assert(notCollidableCountMatch, "colNotCollidable count mismatch");

        if (notCollidableCountMatch)
        {
            for (size_t i = 0; i < origLayer.colNotCollidable.vec.size(); i++)
            {
                const auto &origRect = origLayer.colNotCollidable.vec[i].getColliderRect();
                const auto &loadRect = loadLayer.colNotCollidable.vec[i].getColliderRect();
                bool match = origRect.x == loadRect.x && origRect.y == loadRect.y && origRect.width == loadRect.width && origRect.height == loadRect.height;
                result.assert(match, "colNotCollidable[" + std::to_string(i) + "] mismatch");
            }
        }
    }

    std::set<uint32_t> loadedIds;
    for (const auto &layer : loaded.layers)
    {
        for (const auto &obj : layer.colStatic.vec) loadedIds.insert(obj.id);
        for (const auto &obj : layer.colRigid.vec) loadedIds.insert(obj.id);
        for (const auto &obj : layer.colNotCollidable.vec) loadedIds.insert(obj.id);
    }
    size_t totalObjects = 0;
    for (const auto &layer : loaded.layers)
        totalObjects += layer.colStatic.vec.size() + layer.colRigid.vec.size() + layer.colNotCollidable.vec.size();
    bool allIdsUnique = loadedIds.size() == totalObjects;
    result.assert(allIdsUnique, "Loaded object IDs are not unique");

    std::remove(testFile);

    return result;
}
