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
using namespace dcs;

namespace
{
///////////////////

void testPawnScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more pawns is higher";

      VERIFY(bt(score(Position("wa2 wb2")), score(Position("wa2")), true), caseLabel);
      VERIFY(bt(score(Position("ba7 bb7")), score(Position("ba7")), false), caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Center pawns have higher score";

      VERIFY(bt(score(Position("wb3")), score(Position("wa3")), true), caseLabel);
      VERIFY(bt(score(Position("wc3")), score(Position("wb3")), true), caseLabel);
      VERIFY(bt(score(Position("wd3")), score(Position("wc3")), true), caseLabel);
      VERIFY(bt(score(Position("we3")), score(Position("wf3")), true), caseLabel);
      VERIFY(bt(score(Position("wf3")), score(Position("wg3")), true), caseLabel);
      VERIFY(bt(score(Position("wg3")), score(Position("wh3")), true), caseLabel);

      VERIFY(bt(score(Position("bb5")), score(Position("ba5")), false), caseLabel);
      VERIFY(bt(score(Position("bc5")), score(Position("bb5")), false), caseLabel);
      VERIFY(bt(score(Position("bd5")), score(Position("bc5")), false), caseLabel);
      VERIFY(bt(score(Position("be5")), score(Position("bf5")), false), caseLabel);
      VERIFY(bt(score(Position("bf5")), score(Position("bg5")), false), caseLabel);
      VERIFY(bt(score(Position("bg5")), score(Position("bh5")), false), caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Pawns on deeper ranks have higher score";

      VERIFY(bt(score(Position("wb3")), score(Position("wb2")), true), caseLabel);
      VERIFY(bt(score(Position("wc4")), score(Position("wc3")), true), caseLabel);
      VERIFY(bt(score(Position("wd5")), score(Position("wd4")), true), caseLabel);
      VERIFY(bt(score(Position("we6")), score(Position("we5")), true), caseLabel);
      VERIFY(bt(score(Position("wf7")), score(Position("wf6")), true), caseLabel);

      VERIFY(bt(score(Position("bg6")), score(Position("bg7")), false), caseLabel);
      VERIFY(bt(score(Position("bf5")), score(Position("bf6")), false), caseLabel);
      VERIFY(bt(score(Position("be4")), score(Position("be5")), false), caseLabel);
      VERIFY(bt(score(Position("bd3")), score(Position("bd4")), false), caseLabel);
      VERIFY(bt(score(Position("bc2")), score(Position("bc3")), false), caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Passed pawns have higher score - WhiteIdx";

      const Position passed("wd5 bf7");
      const Position blocked("wd5 bd7");
      const Position neighborA("wd5 bc7");
      const Position neighborB("wd5 be7");

      VERIFY(bt(score(passed), score(blocked), true), caseLabel);
      VERIFY(bt(score(passed), score(neighborA), true), caseLabel);
      VERIFY(bt(score(passed), score(neighborB), true), caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Passed pawns have higher score - BlackIdx";

      const Position passed("bf3 wc2");
      const Position blocked("bf3 wf2");
      const Position neighborA("bf3 we2");
      const Position neighborB("bf3 wg2");

      VERIFY(bt(score(passed), score(blocked), false), caseLabel);
      VERIFY(bt(score(passed), score(neighborA), false), caseLabel);
      VERIFY(bt(score(passed), score(neighborB), false), caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Double pawn penalty - WhiteIdx";

      const Position doublePawns("wg4 wg5");
      const Position individualPawns("wg4 wf5");

      VERIFY(bt(score(individualPawns), score(doublePawns), true), caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Double pawn penalty - BlackIdx";

      const Position doublePawns("be3 be5");
      const Position individualPawns("be3 bd5");

      VERIFY(bt(score(individualPawns), score(doublePawns), false), caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Isolated pawn penalty - WhiteIdx";

      const Position isolatedPawns("wg4 wc5");
      const Position connectedPawns("wg4 wf5");

      VERIFY(bt(score(connectedPawns), score(isolatedPawns), true), caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Isolated pawn penalty - BlackIdx";

      const Position isolatedPawns("bc6 be7");
      const Position connectedPawns("bc6 bd7");

      VERIFY(bt(score(connectedPawns), score(isolatedPawns), false), caseLabel);
   }
}

void testBishopScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more bishops is higher";

      VERIFY(bt(score(Position("Bwe5 Bwd5")), score(Position("Bwe5")), true), caseLabel);
      VERIFY(bt(score(Position("Bbe5 Bbe4")), score(Position("Bbe5")), false), caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Multiple bishops receive a bonus";

      const double multipleScore = score(Position("Bwe5 Bwd5"));
      const double singleScore = score(Position("Bwe5"));
      VERIFY(multipleScore > 2. * singleScore, caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Penality if own pawn is diagonal neighbor of bishop";

      VERIFY(bt(score(Position("Bwe5")) + score(Position("wd4")),
                score(Position("Bwe5 wd4")), true),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Penality if opposing pawn is diagonal neighbor of bishop";

      VERIFY(bt(score(Position("Bbf3")) - score(Position("wg4")),
                score(Position("Bbf3 wg4")), false),
             caseLabel);
   }
}

void testRookScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more rooks is higher";

      VERIFY(bt(score(Position("Rwe5 Rwd5")), score(Position("Rwe5")), true), caseLabel);
      VERIFY(bt(score(Position("Rbe5 Rbe4")), score(Position("Rbe5")), false), caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for rook proximity to enemy king";

      VERIFY(bt(score(Position("Kbd5 Rwe5")), score(Position("Kbd5 Rwf6")), true),
             caseLabel);
      VERIFY(bt(score(Position("Kwh8 Rbb2")), score(Position("Kwh8 Rba1")), false),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Bonus for rook on 7th rank";

      VERIFY(bt(score(Position("Rwd7")), score(Position("Rwd6")), true), caseLabel);
      VERIFY(bt(score(Position("Rbd2")), score(Position("Rbd3")), false), caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for rooks on shared file";

      VERIFY(bt(score(Position("Rwd3 Rwd6")), score(Position("Rwd3 Rwe6")), true),
             caseLabel);
      VERIFY(bt(score(Position("Rbg5 Rbg1")), score(Position("Rbg5 Rbh1")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for no pawns on rook files";

      VERIFY(bt(score(Position("Rwd3 we6")), score(Position("Rwd3 wd6")), true),
             caseLabel);
      VERIFY(bt(score(Position("Rbe5 bd3")), score(Position("Rbe5 be4")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for no pawns on rook files for multiple rooks";

      VERIFY(bt(score(Position("Rwd3 wd6 Rwc1 wf5")),
                score(Position("Rwd3 wd6 Rwc1 wc5")), true),
             caseLabel);
      VERIFY(bt(score(Position("Rba5 ba6 Rbb3 bg6")),
                score(Position("Rba5 ba6 Rbb3 bb6")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for only enemy pawns on rook files";

      VERIFY(bt(score(Position("Rwd3 bd6 we4")), score(Position("Rwd3 bd6 wd4")), true),
             caseLabel);
      VERIFY(bt(score(Position("Rbe5 we3 bd6")), score(Position("Rbe5 we3 be6")), false),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Bonus for only enemy pawns on "
                                    "rook files for multiple rooks";

      VERIFY(bt(score(Position("Rwd3 bd6 Rwf3 bf2 wc4")),
                score(Position("Rwd3 bd6 Rwf3 bf2 wf4")), true),
             caseLabel);
      VERIFY(bt(score(Position("Rbh5 wh3 Rbg5 wg3 bb4")),
                score(Position("Rbh5 wh3 Rbg5 wg3 bg4")), false),
             caseLabel);
   }
}

void testKnightScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more knights is higher";

      VERIFY(bt(score(Position("Nwe5 Nwd5")), score(Position("Nwe5")), true), caseLabel);
      VERIFY(bt(score(Position("Nbe5 Nbe4")), score(Position("Nbe5")), false), caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for knight closeness to center";

      VERIFY(bt(score(Position("Nwb2")), score(Position("Nwa1")), true), caseLabel);
      VERIFY(bt(score(Position("Nwc3")), score(Position("Nwb1")), true), caseLabel);
      VERIFY(bt(score(Position("Nwe5")), score(Position("Nwc3")), true), caseLabel);
      VERIFY(bt(score(Position("Nbg6")), score(Position("Nbh8")), false), caseLabel);
      VERIFY(bt(score(Position("Nbf6")), score(Position("Nbg8")), false), caseLabel);
      VERIFY(bt(score(Position("Nbd4")), score(Position("Nbe6")), false), caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for knight closeness to enemy king";

      // Note - It's a bit tricky to isolate the closeness score from other components of
      //        a knights score, e.g. location on board!
      VERIFY(bt(score(Position("Nwd2 Kbd4")), score(Position("Nwc2 Kbd4")), true),
             caseLabel);
      VERIFY(score(Position("Nwg8 Kbd4")) == score(Position("Nwh7 Kbd4")), caseLabel);
      VERIFY(bt(score(Position("Nbd2 Kwf5")), score(Position("Nbc2 Kwf5")), false),
             caseLabel);
      VERIFY(score(Position("Nbg8 Kwf5")) == score(Position("Nbh7 Kwf5")), caseLabel);
   }
}

void testQueenScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more queens is higher";

      VERIFY(bt(score(Position("Qwe5 Qwd5")), score(Position("Qwe5")), true), caseLabel);
      VERIFY(bt(score(Position("Qbe5 Qbe4")), score(Position("Qbe5")), false), caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for queen proximity to enemy king";

      VERIFY(bt(score(Position("Kbd5 Qwe5")), score(Position("Kbd5 Qwf6")), true),
             caseLabel);
      VERIFY(bt(score(Position("Kwh8 Qbb2")), score(Position("Kwh8 Qba1")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for queen and friendly bishop on same diagonal";

      VERIFY(bt(score(Position("Qwe5 Bwc3")), score(Position("Qwe5 Bwd3")), true),
             caseLabel);
      VERIFY(bt(score(Position("Qbb7 Bbg2")), score(Position("Qbb7 Bbf2")), false),
             caseLabel);
   }
}

void testKingScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Penalty for more enemy pieces in king's quadrant";

      // a1 quadrant with more pieces for black.
      VERIFY(bt(score(Position("Kwa2 wb2 Bbd4 Nbg6")),
                score(Position("Kwa2 wb2 Bbd4 Nba4")), true),
             caseLabel);
      // h1 quadrant with more pieces for black.
      VERIFY(bt(score(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg6")),
                score(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3")), true),
             caseLabel);
      // a8 quadrant with more pieces for white.
      VERIFY(bt(score(Position("Kba8 Nba7 Bwc5 wd4")),
                score(Position("Kba8 Nba7 Bwc5 wd5")), false),
             caseLabel);
      // h8 quadrant with more pieces for white.
      VERIFY(bt(score(Position("Kbg8 Nbf6 be7 Bwf5 we5 Rwg4")),
                score(Position("Kbg8 Nbf6 be7 Bwf5 we5 Rwg6")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - No penalty for more enemy pieces in king's quadrant when "
         "king is in enemy quadrant";

      // White king in a8 quadrant.
      VERIFY(
         !bt(score(Position("Kwa5 Bbd7 Nbe5")), score(Position("Kwa5 Bbd7 Nbc5")), true),
         caseLabel);
      // Black king in h1 quadrant.
      VERIFY(!bt(score(Position("Kbg2 Nbf3 Bwf4 we4 Rwg5")),
                 score(Position("Kbg2 Nbf3 Bwf4 we4 Rwg4")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Higher penalty for more enemy pieces in king's quadrant";

      // White king in h1 quadrant.
      VERIFY(bt(score(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3 Rbc3")),
                score(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3 Rbe3")), true),
             caseLabel);
      // Black king in a8 quadrant.
      VERIFY(
         bt(score(Position("Kbg8 Bwf5 Rwg4")), score(Position("Kbg8 Bwf5 Rwg6")), false),
         caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Queen counts as more than one "
                                    "piece for in king's quadrant penalty";

      // White king in h1 quadrant.
      VERIFY(bt(score(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3 Rbe3")),
                score(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3 Qbe3")), true),
             caseLabel);
      // Black king in a8 quadrant.
      VERIFY(
         bt(score(Position("Kbg8 Bwf5 Rwg6")), score(Position("Kbg8 Bwf5 Qwg6")), false),
         caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Never castled penalty";

      Position hasNotCastled("Kwe2 Rwa1 Rwh1");
      // No castling possible anymore.
      hasNotCastled.hasKingMoved(White);

      Position hasCastled = hasNotCastled;
      hasCastled.setHasCastled(White);

      VERIFY(bt(score(hasCastled), score(hasNotCastled), true), caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Penality when castling possible but rook moved";

      Position original("Kwe1 Rwa1 Rwh1");
      Position kingsideRookMoved("Kwe1 Rwa1 Rwg1");
      kingsideRookMoved.hasRookMoved(White, true);
      Position queensideRookMoved("Kwe1 Rwb1 Rwh1");
      queensideRookMoved.hasRookMoved(White, false);

      VERIFY(bt(score(original), score(kingsideRookMoved), true), caseLabel);
      VERIFY(bt(score(original), score(queensideRookMoved), true), caseLabel);
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
