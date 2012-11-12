#include <test/core_unit_tests.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(CoreTests);

void CoreTests::setUp() {
    m_pTest = new int[10];    
}

void CoreTests::tearDown() {
    if (m_pTest != NULL)
        delete[] m_pTest;
}

void CoreTests::testTest() {
    CPPUNIT_ASSERT_EQUAL(1, 1);
}

