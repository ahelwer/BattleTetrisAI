#include <test/harmony_search_functional_tests.hpp>
#include <test/util.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(HarmonySearchFunctionalTests);

void HarmonySearchFunctionalTests::setUp() {
    m_vCount = 2;
    m_memory = 20;
    m_pRanges = new HarmonyRanges();
    for (unsigned i = 0; i < m_vCount; ++i) {
        m_pRanges->push_back(std::pair<float, float>(-10.0, 10.0));
    }
    m_pFactory = new HarmonyFactory(m_vCount, *m_pRanges);
    m_pFunction = new Rosenbrock();
    m_pCompare = new HarmonyCompareMin(*m_pFunction);
    m_pCompareWrapper = new HarmonyCompareWrapper(*m_pCompare);
    m_pSearch = new HarmonySearch(*m_pCompareWrapper, *m_pFactory, 
                                    m_vCount, m_memory,
                                    0.95, 0.7, 0.05);
}

void HarmonySearchFunctionalTests::tearDown() {
    if (m_pSearch != NULL) {
        delete m_pSearch;
        m_pSearch = NULL;
    }
    if (m_pFactory != NULL) {
        delete m_pFactory;
        m_pFactory = NULL;
    }
    if (m_pRanges != NULL) {
        delete m_pRanges;
        m_pRanges = NULL;
    }
    if (m_pCompareWrapper != NULL) {
        delete m_pCompareWrapper;
        m_pCompareWrapper = NULL;
    }
    if (m_pCompare != NULL) {
        delete m_pCompare;
        m_pCompare = NULL;
    }
    if (m_pFunction != NULL) {
        delete m_pFunction;
        m_pFunction = NULL;
    }
}

void HarmonySearchFunctionalTests::TestRosenbrock() {
    for (unsigned i = 0; i < 20000; ++i) {
        m_pSearch->Iterate();
    }
    Harmony const* best = m_pSearch->GetRanked(0);
    CPPUNIT_ASSERT(FloatEqual(1.0f, best->at(0)));
    CPPUNIT_ASSERT(FloatEqual(1.0f, best->at(1)));
    delete best;
}

