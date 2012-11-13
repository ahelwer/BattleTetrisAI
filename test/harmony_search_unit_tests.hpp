#include <cppunit/extensions/HelperMacros.h>
#include <core/harmony_search.hpp>
#include <core/harmony.hpp>

class SumFunction : public ObjectiveFunction {
public:
	float operator() (Harmony const& h) const {
		float result = 0.0;
		for (unsigned i = 0; i < h.size(); ++i)
			result += h.at(i);
		return result;
	}
};

class PredictableFactory : public HarmonyFactory {
public:
	PredictableFactory(unsigned decisionVarCount, HarmonyRanges ranges)
		: HarmonyFactory(decisionVarCount, ranges), m_gen(0) { }
	virtual Harmony* GenerateRandomHarmony() const {
		Harmony* newHarmony = new Harmony();
		for (unsigned i = 0; i < HarmonyFactory::m_vCount; ++i)
			newHarmony->push_back(HarmonyFactory::m_vCount-m_gen);
		++m_gen;
		return newHarmony;
	}
	virtual float GenerateRandomVariable(unsigned) const {
		return 1.0;
	}
	virtual float ModifyVariableTone(unsigned, float old, float) const {
		return old;
	}
	void Reset() const {
		m_gen = 0;
	}
private:
	mutable unsigned m_gen;
};

class HarmonySearchUnitTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(HarmonySearchUnitTests);
    CPPUNIT_TEST(TestInitialization);
    CPPUNIT_TEST(TestIteration);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
    void TestInitialization();
    void TestIteration();
private:
	unsigned m_vCount;
	unsigned m_memory;
	HarmonyRanges* m_pRanges;
	PredictableFactory* m_pFactory;
	SumFunction* m_pFunction;
	HarmonyCompare* m_pCompare;
	HarmonySearch* m_pSearch;
};

