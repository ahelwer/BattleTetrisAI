#pragma once

#include <cppunit/extensions/HelperMacros.h>

class TetrisOracleUnitTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(TetrisOracleUnitTests);
    CPPUNIT_TEST(TestSquareBlockOnEmptyGrid);
    CPPUNIT_TEST(TestLineBlockOnEmptyGrid);
    CPPUNIT_TEST_SUITE_END();
public:
    void TestSquareBlockOnEmptyGrid();
    void TestLineBlockOnEmptyGrid();
};


