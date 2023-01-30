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

void testIsBetterScore()
{
   {
      const std::string caseLabel = "isBetterScore for max calculation";

      VERIFY(isBetterScore(10., 5., true), caseLabel);
      VERIFY(isBetterScore(-5., -10., true), caseLabel);
   }
   {
      const std::string caseLabel = "isBetterScore for min calculation";

      VERIFY(isBetterScore(5., 10., false), caseLabel);
      VERIFY(isBetterScore(-10., -5., false), caseLabel);
   }
}

void testGetWorstScore()
{
   {
      const std::string caseLabel = "getWorstScoreValue";

      VERIFY(getWorstScoreValue(true) < -100000, caseLabel);
      VERIFY(getWorstScoreValue(false) > 100000, caseLabel);
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
   testIsBetterScore();
   testGetWorstScore();
   testCalcScore();
}
