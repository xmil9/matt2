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

      VERIFY(bt(10., 5., Color::White), caseLabel);
      VERIFY(bt(-5., -10., Color::White), caseLabel);
      VERIFY(!bt(5., 10., Color::White), caseLabel);
   }
   {
      const std::string caseLabel = "bt for black";

      VERIFY(bt(5., 10., Color::Black), caseLabel);
      VERIFY(bt(-10., -5., Color::Black), caseLabel);
      VERIFY(!bt(10., 5., Color::Black), caseLabel);
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

      VERIFY(cmp(10., 5., Color::White) == 1, caseLabel);
      VERIFY(cmp(-5., -10., Color::White) == 1, caseLabel);
      VERIFY(cmp(5., 5., Color::White) == 0, caseLabel);
      VERIFY(cmp(-1., -1., Color::White) == 0, caseLabel);
      VERIFY(cmp(5., 10., Color::White) == -1, caseLabel);
      VERIFY(cmp(-2., 2., Color::White) == -1, caseLabel);
   }
   {
      const std::string caseLabel = "cmp for black";

      VERIFY(cmp(5., 10., Color::Black) == 1, caseLabel);
      VERIFY(cmp(-5., -4., Color::Black) == 1, caseLabel);
      VERIFY(cmp(5., 5., Color::Black) == 0, caseLabel);
      VERIFY(cmp(-1., -1., Color::Black) == 0, caseLabel);
      VERIFY(cmp(5., 1., Color::Black) == -1, caseLabel);
      VERIFY(cmp(2., -2., Color::Black) == -1, caseLabel);
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

      VERIFY(getWorstScoreValue(Color::White) < -100000, caseLabel);
      VERIFY(getWorstScoreValue(Color::Black) > 100000, caseLabel);
   }
}

void testCalcScore()
{
   {
      const std::string caseLabel = "calcScore should return a score";

      VERIFY(calcScore(Position("wa2 wb2")) != 0., caseLabel);
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
}
