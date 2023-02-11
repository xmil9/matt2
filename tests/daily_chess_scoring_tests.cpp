//
// Jan-2023, Michael Lindner
// MIT license
//
#include "daily_chess_scoring_tests.h"
#include "daily_chess_scoring.h"
#include "position.h"
#include "scoring.h"
#include "test_util.h"
#include <stdexcept>

using namespace matt2;


namespace
{
///////////////////

void testPawnScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more pawns is higher";

      VERIFY(bt(calcDailyChessScore(Position("wa2 wb2")),
                calcDailyChessScore(Position("wa2")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("ba7 bb7")),
                calcDailyChessScore(Position("ba7")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Center pawns have higher score";

      VERIFY(bt(calcDailyChessScore(Position("wb3")),
                calcDailyChessScore(Position("wa3")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("wc3")),
                calcDailyChessScore(Position("wb3")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("wd3")),
                calcDailyChessScore(Position("wc3")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("we3")),
                calcDailyChessScore(Position("wf3")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("wf3")),
                calcDailyChessScore(Position("wg3")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("wg3")),
                calcDailyChessScore(Position("wh3")), true),
             caseLabel);

      VERIFY(bt(calcDailyChessScore(Position("bb5")),
                calcDailyChessScore(Position("ba5")), false),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("bc5")),
                calcDailyChessScore(Position("bb5")), false),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("bd5")),
                calcDailyChessScore(Position("bc5")), false),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("be5")),
                calcDailyChessScore(Position("bf5")), false),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("bf5")),
                calcDailyChessScore(Position("bg5")), false),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("bg5")),
                calcDailyChessScore(Position("bh5")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Pawns on deeper ranks have higher score";

      VERIFY(bt(calcDailyChessScore(Position("wb3")),
                calcDailyChessScore(Position("wb2")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("wc4")),
                calcDailyChessScore(Position("wc3")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("wd5")),
                calcDailyChessScore(Position("wd4")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("we6")),
                calcDailyChessScore(Position("we5")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("wf7")),
                calcDailyChessScore(Position("wf6")), true),
             caseLabel);

      VERIFY(bt(calcDailyChessScore(Position("bg6")),
                calcDailyChessScore(Position("bg7")), false),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("bf5")),
                calcDailyChessScore(Position("bf6")), false),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("be4")),
                calcDailyChessScore(Position("be5")), false),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("bd3")),
                calcDailyChessScore(Position("bd4")), false),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("bc2")),
                calcDailyChessScore(Position("bc3")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Passed pawns have higher score - White";

      const Position passed("wd5 bf7");
      const Position blocked("wd5 bd7");
      const Position neighborA("wd5 bc7");
      const Position neighborB("wd5 be7");

      VERIFY(bt(calcDailyChessScore(passed), calcDailyChessScore(blocked), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(passed), calcDailyChessScore(neighborA), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(passed), calcDailyChessScore(neighborB), true),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Passed pawns have higher score - Black";

      const Position passed("bf3 wc2");
      const Position blocked("bf3 wf2");
      const Position neighborA("bf3 we2");
      const Position neighborB("bf3 wg2");

      VERIFY(bt(calcDailyChessScore(passed), calcDailyChessScore(blocked), false),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(passed), calcDailyChessScore(neighborA), false),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(passed), calcDailyChessScore(neighborB), false),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Double pawn penalty - White";

      const Position doublePawns("wg4 wg5");
      const Position individualPawns("wg4 wf5");

      VERIFY(
         bt(calcDailyChessScore(individualPawns), calcDailyChessScore(doublePawns), true),
         caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Double pawn penalty - Black";

      const Position doublePawns("be3 be5");
      const Position individualPawns("be3 bd5");

      VERIFY(bt(calcDailyChessScore(individualPawns), calcDailyChessScore(doublePawns),
                false),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Isolated pawn penalty - White";

      const Position isolatedPawns("wg4 wc5");
      const Position connectedPawns("wg4 wf5");

      VERIFY(bt(calcDailyChessScore(connectedPawns), calcDailyChessScore(isolatedPawns),
                true),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Isolated pawn penalty - Black";

      const Position isolatedPawns("bc6 be7");
      const Position connectedPawns("bc6 bd7");

      VERIFY(bt(calcDailyChessScore(connectedPawns), calcDailyChessScore(isolatedPawns),
                false),
             caseLabel);
   }
}

void testBishopScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more bishops is higher";

      VERIFY(bt(calcDailyChessScore(Position("Bwe5 Bwd5")),
                calcDailyChessScore(Position("Bwe5")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Bbe5 Bbe4")),
                calcDailyChessScore(Position("Bbe5")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Multiple bishops receive a bonus";

      const double multipleScore = calcDailyChessScore(Position("Bwe5 Bwd5"));
      const double singleScore = calcDailyChessScore(Position("Bwe5"));
      VERIFY(multipleScore > 2. * singleScore, caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Penality if own pawn is diagonal neighbor of bishop";

      VERIFY(
         bt(calcDailyChessScore(Position("Bwe5")) + calcDailyChessScore(Position("wd4")),
            calcDailyChessScore(Position("Bwe5 wd4")), true),
         caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Penality if opposing pawn is diagonal neighbor of bishop";

      VERIFY(
         bt(calcDailyChessScore(Position("Bbf3")) - calcDailyChessScore(Position("wg4")),
            calcDailyChessScore(Position("Bbf3 wg4")), false),
         caseLabel);
   }
}

void testRookScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more rooks is higher";

      VERIFY(bt(calcDailyChessScore(Position("Rwe5 Rwd5")),
                calcDailyChessScore(Position("Rwe5")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Rbe5 Rbe4")),
                calcDailyChessScore(Position("Rbe5")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for rook proximity to enemy king";

      VERIFY(bt(calcDailyChessScore(Position("Kbd5 Rwe5")),
                calcDailyChessScore(Position("Kbd5 Rwf6")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Kwh8 Rbb2")),
                calcDailyChessScore(Position("Kwh8 Rba1")), false),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Bonus for rook on 7th rank";

      VERIFY(bt(calcDailyChessScore(Position("Rwd7")),
                calcDailyChessScore(Position("Rwd6")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Rbd2")),
                calcDailyChessScore(Position("Rbd3")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for rooks on shared file";

      VERIFY(bt(calcDailyChessScore(Position("Rwd3 Rwd6")),
                calcDailyChessScore(Position("Rwd3 Rwe6")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Rbg5 Rbg1")),
                calcDailyChessScore(Position("Rbg5 Rbh1")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for no pawns on rook files";

      VERIFY(bt(calcDailyChessScore(Position("Rwd3 we6")),
                calcDailyChessScore(Position("Rwd3 wd6")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Rbe5 bd3")),
                calcDailyChessScore(Position("Rbe5 be4")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for no pawns on rook files for multiple rooks";

      VERIFY(bt(calcDailyChessScore(Position("Rwd3 wd6 Rwc1 wf5")),
                calcDailyChessScore(Position("Rwd3 wd6 Rwc1 wc5")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Rba5 ba6 Rbb3 bg6")),
                calcDailyChessScore(Position("Rba5 ba6 Rbb3 bb6")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for only enemy pawns on rook files";

      VERIFY(bt(calcDailyChessScore(Position("Rwd3 bd6 we4")),
                calcDailyChessScore(Position("Rwd3 bd6 wd4")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Rbe5 we3 bd6")),
                calcDailyChessScore(Position("Rbe5 we3 be6")), false),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Bonus for only enemy pawns on "
                                    "rook files for multiple rooks";

      VERIFY(bt(calcDailyChessScore(Position("Rwd3 bd6 Rwf3 bf2 wc4")),
                calcDailyChessScore(Position("Rwd3 bd6 Rwf3 bf2 wf4")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Rbh5 wh3 Rbg5 wg3 bb4")),
                calcDailyChessScore(Position("Rbh5 wh3 Rbg5 wg3 bg4")), false),
             caseLabel);
   }
}

void testKnightScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more knights is higher";

      VERIFY(bt(calcDailyChessScore(Position("Nwe5 Nwd5")),
                calcDailyChessScore(Position("Nwe5")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Nbe5 Nbe4")),
                calcDailyChessScore(Position("Nbe5")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for knight closeness to center";

      VERIFY(bt(calcDailyChessScore(Position("Nwb2")),
                calcDailyChessScore(Position("Nwa1")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Nwc3")),
                calcDailyChessScore(Position("Nwb1")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Nwe5")),
                calcDailyChessScore(Position("Nwc3")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Nbg6")),
                calcDailyChessScore(Position("Nbh8")), false),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Nbf6")),
                calcDailyChessScore(Position("Nbg8")), false),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Nbd4")),
                calcDailyChessScore(Position("Nbe6")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for knight closeness to enemy king";

      // Note - It's a bit tricky to isolate the closeness score from other components of
      //        a knights score, e.g. location on board!
      VERIFY(bt(calcDailyChessScore(Position("Nwd2 Kbd4")),
                calcDailyChessScore(Position("Nwc2 Kbd4")), true),
             caseLabel);
      VERIFY(calcDailyChessScore(Position("Nwg8 Kbd4")) ==
                calcDailyChessScore(Position("Nwh7 Kbd4")),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Nbd2 Kwf5")),
                calcDailyChessScore(Position("Nbc2 Kwf5")), false),
             caseLabel);
      VERIFY(calcDailyChessScore(Position("Nbg8 Kwf5")) ==
                calcDailyChessScore(Position("Nbh7 Kwf5")),
             caseLabel);
   }
}

void testQueenScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more queens is higher";

      VERIFY(bt(calcDailyChessScore(Position("Qwe5 Qwd5")),
                calcDailyChessScore(Position("Qwe5")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Qbe5 Qbe4")),
                calcDailyChessScore(Position("Qbe5")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for queen proximity to enemy king";

      VERIFY(bt(calcDailyChessScore(Position("Kbd5 Qwe5")),
                calcDailyChessScore(Position("Kbd5 Qwf6")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Kwh8 Qbb2")),
                calcDailyChessScore(Position("Kwh8 Qba1")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for queen and friendly bishop on same diagonal";

      VERIFY(bt(calcDailyChessScore(Position("Qwe5 Bwc3")),
                calcDailyChessScore(Position("Qwe5 Bwd3")), true),
             caseLabel);
      VERIFY(bt(calcDailyChessScore(Position("Qbb7 Bbg2")),
                calcDailyChessScore(Position("Qbb7 Bbf2")), false),
             caseLabel);
   }
}

void testKingScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Penalty for more enemy pieces in king's quadrant";

      // a1 quadrant with more pieces for black.
      VERIFY(bt(calcDailyChessScore(Position("Kwa2 wb2 Bbd4 Nbg6")),
                calcDailyChessScore(Position("Kwa2 wb2 Bbd4 Nba4")), true),
             caseLabel);
      // h1 quadrant with more pieces for black.
      VERIFY(bt(calcDailyChessScore(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg6")),
                calcDailyChessScore(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3")), true),
             caseLabel);
      // a8 quadrant with more pieces for white.
      VERIFY(bt(calcDailyChessScore(Position("Kba8 Nba7 Bwc5 wd4")),
                calcDailyChessScore(Position("Kba8 Nba7 Bwc5 wd5")), false),
             caseLabel);
      // h8 quadrant with more pieces for white.
      VERIFY(bt(calcDailyChessScore(Position("Kbg8 Nbf6 be7 Bwf5 we5 Rwg4")),
                calcDailyChessScore(Position("Kbg8 Nbf6 be7 Bwf5 we5 Rwg6")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - No penalty for more enemy pieces in king's quadrant when "
         "king is in enemy quadrant";

      // White king in a8 quadrant.
      VERIFY(!bt(calcDailyChessScore(Position("Kwa5 Bbd7 Nbe5")),
                 calcDailyChessScore(Position("Kwa5 Bbd7 Nbc5")), true),
             caseLabel);
      // Black king in h1 quadrant.
      VERIFY(!bt(calcDailyChessScore(Position("Kbg2 Nbf3 Bwf4 we4 Rwg5")),
                 calcDailyChessScore(Position("Kbg2 Nbf3 Bwf4 we4 Rwg4")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Higher penalty for more enemy pieces in king's quadrant";

      // White king in h1 quadrant.
      VERIFY(bt(calcDailyChessScore(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3 Rbc3")),
                calcDailyChessScore(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3 Rbe3")), true),
             caseLabel);
      // Black king in a8 quadrant.
      VERIFY(bt(calcDailyChessScore(Position("Kbg8 Bwf5 Rwg4")),
                calcDailyChessScore(Position("Kbg8 Bwf5 Rwg6")), false),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Queen counts as more than one "
                                    "piece for in king's quadrant penalty";

      // White king in h1 quadrant.
      VERIFY(bt(calcDailyChessScore(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3 Rbe3")),
                calcDailyChessScore(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3 Qbe3")), true),
             caseLabel);
      // Black king in a8 quadrant.
      VERIFY(bt(calcDailyChessScore(Position("Kbg8 Bwf5 Rwg6")),
                calcDailyChessScore(Position("Kbg8 Bwf5 Qwg6")), false),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Never castled penalty";

      Position hasNotCastled("Kwe2 Rwa1 Rwh1");
      // No castling possible anymore.
      hasNotCastled.hasKingMoved(Color::White);

      Position hasCastled = hasNotCastled;
      hasCastled.setHasCastled(Color::White);

      VERIFY(
         bt(calcDailyChessScore(hasCastled), calcDailyChessScore(hasNotCastled), true),
         caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Penality when castling possible but rook moved";

      Position original("Kwe1 Rwa1 Rwh1");
      Position kingsideRookMoved("Kwe1 Rwa1 Rwg1");
      kingsideRookMoved.hasRookMoved(Color::White, true);
      Position queensideRookMoved("Kwe1 Rwb1 Rwh1");
      queensideRookMoved.hasRookMoved(Color::White, false);

      VERIFY(
         bt(calcDailyChessScore(original), calcDailyChessScore(kingsideRookMoved), true),
         caseLabel);
      VERIFY(
         bt(calcDailyChessScore(original), calcDailyChessScore(queensideRookMoved), true),
         caseLabel);
   }
}

} // namespace

///////////////////

void testDailyChessScoring()
{
   testPawnScoring();
   testBishopScoring();
   testRookScoring();
   testKnightScoring();
   testQueenScoring();
   testKingScoring();
}
