#include <test/core_unit_tests.hpp>
#include <test/util.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(HarmonySearchUnitTests);

void HarmonySearchUnitTests::setUp() {
	m_vCount = 5;
	m_memory = 5;
	m_pRanges = new HarmonyRanges();
	for (unsigned i = 0; i < m_vCount; ++i) {
		m_pRanges->push_back(std::pair<float, float>(0.0, 1.0));
	}
	m_pFactory = new PredictableFactory(m_vCount, *m_pRanges);
	m_pFunction = new SumFunction();
	m_pCompare = new HarmonyCompare(*m_pFunction);
	m_pSearch = new HarmonySearch(*m_pFunction, *m_pFactory, 
									m_vCount, m_memory,
									1.0, 0.0, 1.0);
	m_pFactory->Reset();
}

void HarmonySearchUnitTests::tearDown() {
	if (m_pRanges != NULL)
		delete m_pRanges;
	if (m_pFactory != NULL)
		delete m_pFactory;
	if (m_pFunction != NULL)
		delete m_pFunction;
	if (m_pCompare != NULL)
		delete m_pCompare;
	if (m_pSearch != NULL)
		delete m_pSearch;
}

void HarmonySearchUnitTests::TestInitialization() {
	HarmonyCompare const& hc = *m_pCompare;
	// Tests that Harmonies are ordered correctly
	for (unsigned i = 0; i < m_memory-1; ++i) {
		Harmony const& r1 = m_pSearch->GetRanked(i);
		Harmony const& r2 = m_pSearch->GetRanked(i+1);
		bool success = hc(&r1, &r2);
		CPPUNIT_ASSERT(success);
	}
	// Tests that Harmonies are intact as expected
	for (unsigned i = 0; i < m_memory; ++i) {
		Harmony* h1 = m_pFactory->GenerateRandomHarmony();
		Harmony const& h2 = m_pSearch->GetRanked(m_memory-i-1);
		for (unsigned j = 0; j < m_vCount; ++j) {
			float v1 = h1->at(j);
			float v2 = h2.at(j);
			CPPUNIT_ASSERT(FloatEqual(v1, v2));
		}
		delete h1;
	}
	m_pFactory->Reset();
}

#include <iostream>

void HarmonySearchUnitTests::TestIteration() {
	HarmonyCompare const& hc = *m_pCompare;
	Harmony best (m_pSearch->GetRanked(0));
	std::cout << best << std::endl;
	for (unsigned int iter = 0; iter < 100; ++iter) {
		m_pSearch->Iterate();
		for (unsigned i = 0; i < m_memory-1; ++i) {
			Harmony const& r1 = m_pSearch->GetRanked(i);
			Harmony const& r2 = m_pSearch->GetRanked(i+1);
			bool success = hc(&r1, &r2);
			CPPUNIT_ASSERT(success);
		}
		Harmony const& currBest = m_pSearch->GetRanked(0);
		//std::cout << currBest << std::endl;
		for (unsigned i = 0; i < m_vCount; ++i) {
			float v1 = best.at(i);
			float v2 = currBest.at(i);
			CPPUNIT_ASSERT(FloatEqual(v1, v2));
		}
	}
}

