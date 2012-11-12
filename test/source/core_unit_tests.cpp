#include <test/core_unit_tests.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(HarmonySearchUnitTests);

void HarmonySearchUnitTests::setUp() {
	m_vCount = 5;
	m_memory = 5;
	m_pRanges = new HarmonyRanges();
	for (unsigned i = 0; i < m_vCount; ++i) {
		m_pRanges->push_back(std::pair<float, float>(0.0, 1.0));
	}
	m_pPf = new PredictableFactory(m_vCount, *m_pRanges);
	m_pSf = new SumFunction();
	m_pHc = new HarmonyCompare(*m_pSf);
	m_pSearch = new HarmonySearch(*m_pSf, *m_pPf, m_vCount, m_memory,
									1.0, 0.0, 1.0);
	m_pPf->Reset();
}

void HarmonySearchUnitTests::tearDown() {
	delete m_pRanges;
	delete m_pPf;
	delete m_pSf;
	delete m_pHc;
	delete m_pSearch;
}

void HarmonySearchUnitTests::TestInitialization() {
	HarmonyCompare const& hc = *m_pHc;
	for (unsigned i = 0; i < m_memory-1; ++i) {
		Harmony const& r1 = m_pSearch->GetRanked(i);
		Harmony const& r2 = m_pSearch->GetRanked(i+1);
		for (unsigned j = 0; j < m_vCount; ++j) {
			bool success = hc(&r1, &r2);
			CPPUNIT_ASSERT(success);
		}
	}
}

