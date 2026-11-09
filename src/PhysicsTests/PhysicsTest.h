#pragma once
#include <functional>
#include <vector>
#include "PhysicsTest_Result.h"

struct PhysicsTest_Entry {
    const char* name;
    std::function<PhysicsTest_Result()> func;
};

struct PhysicsTest_EntriesRegistrar {
    static std::vector<PhysicsTest_Entry>& getTests() {
        static std::vector<PhysicsTest_Entry> tests;
        return tests;
    }
    
    PhysicsTest_EntriesRegistrar(const char* name, PhysicsTest_Result (*func)()) {
        getTests().push_back({name, func});
    }
};

#ifdef PHYSICS_TEST
#error "PHYSICS_TEST MACRO REDIFINITION"
#endif

#define PHYSICS_TEST(name) \
    PhysicsTest_Result PhysicsTestRun_##name(); \
    static PhysicsTest_EntriesRegistrar _reg_physicsTest_##name(#name, PhysicsTestRun_##name); \
    PhysicsTest_Result PhysicsTestRun_##name()