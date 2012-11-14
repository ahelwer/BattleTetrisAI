#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <core/harmony_search.hpp>
#include <core/harmony.hpp>

class Rosenbrock : public ObjectiveFunction {
public:
    float operator() (Harmony const& h) const {
        float x = h.at(0);
        float y = h.at(1);
        float result = (1-x)*(1-x) + 100*(y-x*x)*(y-x*x);
        return result;
    }
};

class HarmonySearchFunctionalTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(HarmonySearchFunctionalTests);
    CPPUNIT_TEST(TestRosenbrock);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
    void TestRosenbrock();
private:
    unsigned m_vCount;
    unsigned m_memory;
    HarmonyRanges* m_pRanges;
    HarmonyFactory* m_pFactory;
    Rosenbrock* m_pFunction;
    HarmonyCompare* m_pCompare;
    HarmonyCompareWrapper* m_pCompareWrapper;
    HarmonySearch* m_pSearch;
};

