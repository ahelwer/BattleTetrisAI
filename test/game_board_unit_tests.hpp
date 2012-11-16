#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <model/game_board.hpp>

class GameBoardUnitTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(GameBoardUnitTests);
    CPPUNIT_TEST(TestInit);
    CPPUNIT_TEST(TestTranslateEmpty);
    CPPUNIT_TEST(TestTranslateFull);
    CPPUNIT_TEST(TestTranslateComplex);
    CPPUNIT_TEST(TestApplyPiece);
    CPPUNIT_TEST(TestApplyMultiplePieces);
    CPPUNIT_TEST(TestClearRows);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
    void TestInit();
    void TestTranslateEmpty();
    void TestTranslateFull();
    void TestTranslateComplex();
    void TestApplyPiece();
    void TestApplyMultiplePieces();
    void TestClearRows();
private:
    GameBoard* m_pBoard;
};

