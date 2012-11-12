#include <cppunit/extensions/HelperMacros.h>

class CoreTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(CoreTests);
    CPPUNIT_TEST(testTest);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
    void testTest();
private:
    int* m_pTest;
};

