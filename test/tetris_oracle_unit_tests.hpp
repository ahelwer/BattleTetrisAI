#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <model/tetromino.hpp>
#include <model/game_state.hpp>
#include <util/vector.hpp>

class TetrisOracleUnitTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(TetrisOracleUnitTests);
    CPPUNIT_TEST(TestSquareBlockOnEmptyGrid);
    CPPUNIT_TEST(TestLineBlockOnEmptyGrid);
    CPPUNIT_TEST(TestFindBestMove);
    CPPUNIT_TEST(TestFindBestMoveSquareBlockEmptyGrid);
    CPPUNIT_TEST(TestFindPath);
    CPPUNIT_TEST(TestSourceTargetEqual);
    CPPUNIT_TEST(TestFindPathUnderOverhang);
    CPPUNIT_TEST(TestFindPathSlideIn);
    CPPUNIT_TEST(TestNoPath);
    CPPUNIT_TEST_SUITE_END();
public:
    void TestSquareBlockOnEmptyGrid();
    void TestLineBlockOnEmptyGrid();
    void TestFindBestMove();
    void TestFindBestMoveSquareBlockEmptyGrid();
    int SimulatePath(GameState const& state,
                        PathSequence const* path, 
                        Tetromino const& source,
                        Tetromino const& target);
    void TestFindPath();
    void TestSourceTargetEqual();
    void TestFindPathUnderOverhang();
    void TestFindPathSlideIn();
    void TestNoPath();
};


