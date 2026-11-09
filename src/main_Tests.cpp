
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
            Console::log("[TEST PASSED] {}\n", testResult.testName);
        else
            Console::log("[TEST FAIL] {}\n", testResult.testName);
            
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
