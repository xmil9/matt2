//
// Dec-2022, Michael Lindner
// MIT license
//
#include "scoring_tests.h"
#include "position.h"
#include "scoring.h"
#include "test_util.h"
#include <stdexcept>

using namespace matt2;


namespace
{
///////////////////

void testBetterScoreForMinMaxFlag()
{
   {
      const std::string caseLabel = "bt for max calculation";

      VERIFY(bt(10., 5., true), caseLabel);
      VERIFY(bt(-5., -10., true), caseLabel);
      VERIFY(!bt(5., 10., true), caseLabel);
   }
   {
      const std::string caseLabel = "bt for min calculation";

      VERIFY(bt(5., 10., false), caseLabel);
      VERIFY(bt(-10., -5., false), caseLabel);
      VERIFY(!bt(10., 5., false), caseLabel);
   }
}

void testBetterScoreForSide()
{
   {
      const std::string caseLabel = "bt for white";

      VERIFY(bt(10., 5., White), caseLabel);
      VERIFY(bt(-5., -10., White), caseLabel);
      VERIFY(!bt(5., 10., White), caseLabel);
   }
   {
      const std::string caseLabel = "bt for black";

      VERIFY(bt(5., 10., Black), caseLabel);
      VERIFY(bt(-10., -5., Black), caseLabel);
      VERIFY(!bt(10., 5., Black), caseLabel);
   }
}

void testCmpScoreForMinMaxFlag()
{
   {
      const std::string caseLabel = "cmp for max calculation";

      VERIFY(cmp(10., 5., true) == 1, caseLabel);
      VERIFY(cmp(-5., -10., true) == 1, caseLabel);
      VERIFY(cmp(5., 5., true) == 0, caseLabel);
      VERIFY(cmp(-1., -1., true) == 0, caseLabel);
      VERIFY(cmp(5., 10., true) == -1, caseLabel);
      VERIFY(cmp(-2., 2., true) == -1, caseLabel);
   }
   {
      const std::string caseLabel = "cmp for min calculation";

      VERIFY(cmp(5., 10., false) == 1, caseLabel);
      VERIFY(cmp(-5., -4., false) == 1, caseLabel);
      VERIFY(cmp(5., 5., false) == 0, caseLabel);
      VERIFY(cmp(-1., -1., false) == 0, caseLabel);
      VERIFY(cmp(5., 1., false) == -1, caseLabel);
      VERIFY(cmp(2., -2., false) == -1, caseLabel);
   }
}

void testCmpScoreForSide()
{
   {
      const std::string caseLabel = "cmp for white";

      VERIFY(cmp(10., 5., White) == 1, caseLabel);
      VERIFY(cmp(-5., -10., White) == 1, caseLabel);
      VERIFY(cmp(5., 5., White) == 0, caseLabel);
      VERIFY(cmp(-1., -1., White) == 0, caseLabel);
      VERIFY(cmp(5., 10., White) == -1, caseLabel);
      VERIFY(cmp(-2., 2., White) == -1, caseLabel);
   }
   {
      const std::string caseLabel = "cmp for black";

      VERIFY(cmp(5., 10., Black) == 1, caseLabel);
      VERIFY(cmp(-5., -4., Black) == 1, caseLabel);
      VERIFY(cmp(5., 5., Black) == 0, caseLabel);
      VERIFY(cmp(-1., -1., Black) == 0, caseLabel);
      VERIFY(cmp(5., 1., Black) == -1, caseLabel);
      VERIFY(cmp(2., -2., Black) == -1, caseLabel);
   }
}

void testGetWorstScoreForMinMaxFlag()
{
   {
      const std::string caseLabel = "getWorstScoreValue for min-max flag";

      VERIFY(getWorstScoreValue(true) < -100000, caseLabel);
      VERIFY(getWorstScoreValue(false) > 100000, caseLabel);
   }
}

void testGetWorstScoreForSide()
{
   {
      const std::string caseLabel = "getWorstScoreValue for side";

      VERIFY(getWorstScoreValue(White) < -100000, caseLabel);
      VERIFY(getWorstScoreValue(Black) > 100000, caseLabel);
   }
}

void testCalcScore()
{
   {
      const std::string caseLabel = "calcScore should return a score";

      VERIFY(calcScore(Position("wa2 wb2")) != 0., caseLabel);
   }
}

void testCalcMateScore()
{
   {
      const std::string caseLabel = "calcMateScore should return a score";

      VERIFY(calcMateScore(White, Position("wa2 wb2"), 0) != 0., caseLabel);
   }
}

void testCalcTieScore()
{
   {
      const std::string caseLabel = "calcTieScore should return a score";

      VERIFY(calcTieScore(White, Position("Kwe1 wa2 wb2 Kbh6")) != 0., caseLabel);
   }
}

} // namespace


///////////////////

void testScoring()
{
   testBetterScoreForMinMaxFlag();
   testBetterScoreForSide();
   testCmpScoreForMinMaxFlag();
   testCmpScoreForSide();
   testGetWorstScoreForMinMaxFlag();
   testGetWorstScoreForSide();
   testCalcScore();
   testCalcMateScore();
   testCalcTieScore();
}
