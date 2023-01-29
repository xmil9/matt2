//
// Dec-2022, Michael Lindner
// MIT license
//
#include "scoring_tests.h"
#include "daily_chess_scoring.h"
#include "piece_value_scoring.h"
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

///////////////////

void testPVPawnScoring()
{
   {
      const std::string caseLabel =
         "Piece value scoring - Score for more pawns is higher";

      VERIFY(calcPieceValueScore(Position("wa2 wb2")) >
                calcPieceValueScore(Position("wa2")),
             caseLabel);
   }
}

void testPVKnightScoring()
{
   {
      const std::string caseLabel =
         "Piece value scoring - Score for more knights is higher";

      VERIFY(calcPieceValueScore(Position("Nwg2 Nwc7")) >
                calcPieceValueScore(Position("Nwg2")),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Piece value scoring - Score for knight is higher than pawn";

      VERIFY(calcPieceValueScore(Position("Nwg2")) > calcPieceValueScore(Position("wf2")),
             caseLabel);
   }
}

void testPVBishopScoring()
{
   {
      const std::string caseLabel =
         "Piece value scoring - Score for more bishops is higher";

      VERIFY(calcPieceValueScore(Position("Bwg2 Bwc7")) >
                calcPieceValueScore(Position("Bwg2")),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Piece value scoring - Score for bishop is higher than pawn";

      VERIFY(calcPieceValueScore(Position("Bwg2")) > calcPieceValueScore(Position("wf2")),
             caseLabel);
   }
}

void testPVRookScoring()
{
   {
      const std::string caseLabel =
         "Piece value scoring - Score for more rooks is higher";

      VERIFY(calcPieceValueScore(Position("Rwg2 Rwc7")) >
                calcPieceValueScore(Position("Rwg2")),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Piece value scoring - Score for rook is higher than pawn";

      VERIFY(calcPieceValueScore(Position("Rwg2")) > calcPieceValueScore(Position("wf2")),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Piece value scoring - Score for rook is higher than knight";

      VERIFY(calcPieceValueScore(Position("Rwg2")) >
                calcPieceValueScore(Position("Nwf2")),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Piece value scoring - Score for rook is higher than bishop";

      VERIFY(calcPieceValueScore(Position("Rwg2")) >
                calcPieceValueScore(Position("Bwf2")),
             caseLabel);
   }
}

void testPVQueenScoring()
{
   {
      const std::string caseLabel =
         "Piece value scoring - Score for queen is higher than pawn";

      VERIFY(calcPieceValueScore(Position("Qwg2")) > calcPieceValueScore(Position("wf2")),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Piece value scoring - Score for queen is higher than knight";

      VERIFY(calcPieceValueScore(Position("Qwg2")) >
                calcPieceValueScore(Position("Nwf2")),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Piece value scoring - Score for queen is higher than bishop";

      VERIFY(calcPieceValueScore(Position("Qwg2")) >
                calcPieceValueScore(Position("Bwf2")),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Piece value scoring - Score for queen is higher than rook";

      VERIFY(calcPieceValueScore(Position("Qwg2")) >
                calcPieceValueScore(Position("Rwf2")),
             caseLabel);
   }
}

void testPVKingScoring()
{
   {
      const std::string caseLabel =
         "Piece value scoring - Score for king is higher than any other piece";

      VERIFY(calcPieceValueScore(Position("Kwg2")) > calcPieceValueScore(Position("wf2")),
             caseLabel);
      VERIFY(calcPieceValueScore(Position("Kwg2")) >
                calcPieceValueScore(Position("Nwf2")),
             caseLabel);
      VERIFY(calcPieceValueScore(Position("Kwg2")) >
                calcPieceValueScore(Position("Bwf2")),
             caseLabel);
      VERIFY(calcPieceValueScore(Position("Kwg2")) >
                calcPieceValueScore(Position("Rwf2")),
             caseLabel);
      VERIFY(calcPieceValueScore(Position("Kwg2")) >
                calcPieceValueScore(Position("Qwf2")),
             caseLabel);
   }
}

void testPieceValueScoring()
{
   testPVPawnScoring();
   testPVKnightScoring();
   testPVBishopScoring();
   testPVRookScoring();
   testPVQueenScoring();
   testPVKingScoring();
}

///////////////////

void testDCPawnScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more pawns is higher";

      VERIFY(isBetterScore(calcDailyChessScore(Position("wa2 wb2")),
                           calcDailyChessScore(Position("wa2")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("ba7 bb7")),
                           calcDailyChessScore(Position("ba7")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Center pawns have higher score";

      VERIFY(isBetterScore(calcDailyChessScore(Position("wb3")),
                           calcDailyChessScore(Position("wa3")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("wc3")),
                           calcDailyChessScore(Position("wb3")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("wd3")),
                           calcDailyChessScore(Position("wc3")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("we3")),
                           calcDailyChessScore(Position("wf3")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("wf3")),
                           calcDailyChessScore(Position("wg3")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("wg3")),
                           calcDailyChessScore(Position("wh3")), true),
             caseLabel);

      VERIFY(isBetterScore(calcDailyChessScore(Position("bb5")),
                           calcDailyChessScore(Position("ba5")), false),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("bc5")),
                           calcDailyChessScore(Position("bb5")), false),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("bd5")),
                           calcDailyChessScore(Position("bc5")), false),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("be5")),
                           calcDailyChessScore(Position("bf5")), false),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("bf5")),
                           calcDailyChessScore(Position("bg5")), false),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("bg5")),
                           calcDailyChessScore(Position("bh5")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Pawns on deeper ranks have higher score";

      VERIFY(isBetterScore(calcDailyChessScore(Position("wb3")),
                           calcDailyChessScore(Position("wb2")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("wc4")),
                           calcDailyChessScore(Position("wc3")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("wd5")),
                           calcDailyChessScore(Position("wd4")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("we6")),
                           calcDailyChessScore(Position("we5")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("wf7")),
                           calcDailyChessScore(Position("wf6")), true),
             caseLabel);

      VERIFY(isBetterScore(calcDailyChessScore(Position("bg6")),
                           calcDailyChessScore(Position("bg7")), false),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("bf5")),
                           calcDailyChessScore(Position("bf6")), false),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("be4")),
                           calcDailyChessScore(Position("be5")), false),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("bd3")),
                           calcDailyChessScore(Position("bd4")), false),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("bc2")),
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

      VERIFY(
         isBetterScore(calcDailyChessScore(passed), calcDailyChessScore(blocked), true),
         caseLabel);
      VERIFY(
         isBetterScore(calcDailyChessScore(passed), calcDailyChessScore(neighborA), true),
         caseLabel);
      VERIFY(
         isBetterScore(calcDailyChessScore(passed), calcDailyChessScore(neighborB), true),
         caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Passed pawns have higher score - Black";

      const Position passed("bf3 wc2");
      const Position blocked("bf3 wf2");
      const Position neighborA("bf3 we2");
      const Position neighborB("bf3 wg2");

      VERIFY(
         isBetterScore(calcDailyChessScore(passed), calcDailyChessScore(blocked), false),
         caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(passed), calcDailyChessScore(neighborA),
                           false),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(passed), calcDailyChessScore(neighborB),
                           false),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Double pawn penalty - White";

      const Position doublePawns("wg4 wg5");
      const Position individualPawns("wg4 wf5");

      VERIFY(isBetterScore(calcDailyChessScore(individualPawns),
                           calcDailyChessScore(doublePawns), true),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Double pawn penalty - Black";

      const Position doublePawns("be3 be5");
      const Position individualPawns("be3 bd5");

      VERIFY(isBetterScore(calcDailyChessScore(individualPawns),
                           calcDailyChessScore(doublePawns), false),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Isolated pawn penalty - White";

      const Position isolatedPawns("wg4 wc5");
      const Position connectedPawns("wg4 wf5");

      VERIFY(isBetterScore(calcDailyChessScore(connectedPawns),
                           calcDailyChessScore(isolatedPawns), true),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Isolated pawn penalty - Black";

      const Position isolatedPawns("bc6 be7");
      const Position connectedPawns("bc6 bd7");

      VERIFY(isBetterScore(calcDailyChessScore(connectedPawns),
                           calcDailyChessScore(isolatedPawns), false),
             caseLabel);
   }
}

void testDCBishopScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more bishops is higher";

      VERIFY(isBetterScore(calcDailyChessScore(Position("Bwe5 Bwd5")),
                           calcDailyChessScore(Position("Bwe5")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Bbe5 Bbe4")),
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

      VERIFY(isBetterScore(calcDailyChessScore(Position("Bwe5")) +
                              calcDailyChessScore(Position("wd4")),
                           calcDailyChessScore(Position("Bwe5 wd4")), true),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Penality if opposing pawn is diagonal neighbor of bishop";

      VERIFY(isBetterScore(calcDailyChessScore(Position("Bbf3")) -
                              calcDailyChessScore(Position("wg4")),
                           calcDailyChessScore(Position("Bbf3 wg4")), false),
             caseLabel);
   }
}

void testDCRookScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more rooks is higher";

      VERIFY(isBetterScore(calcDailyChessScore(Position("Rwe5 Rwd5")),
                           calcDailyChessScore(Position("Rwe5")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Rbe5 Rbe4")),
                           calcDailyChessScore(Position("Rbe5")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for rook proximity to enemy king";

      VERIFY(isBetterScore(calcDailyChessScore(Position("Kbd5 Rwe5")),
                           calcDailyChessScore(Position("Kbd5 Rwf6")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Kwh8 Rbb2")),
                           calcDailyChessScore(Position("Kwh8 Rba1")), false),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Bonus for rook on 7th rank";

      VERIFY(isBetterScore(calcDailyChessScore(Position("Rwd7")),
                           calcDailyChessScore(Position("Rwd6")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Rbd2")),
                           calcDailyChessScore(Position("Rbd3")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for rooks on shared file";

      VERIFY(isBetterScore(calcDailyChessScore(Position("Rwd3 Rwd6")),
                           calcDailyChessScore(Position("Rwd3 Rwe6")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Rbg5 Rbg1")),
                           calcDailyChessScore(Position("Rbg5 Rbh1")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for no pawns on rook files";

      VERIFY(isBetterScore(calcDailyChessScore(Position("Rwd3 we6")),
                           calcDailyChessScore(Position("Rwd3 wd6")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Rbe5 bd3")),
                           calcDailyChessScore(Position("Rbe5 be4")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for no pawns on rook files for multiple rooks";

      VERIFY(isBetterScore(calcDailyChessScore(Position("Rwd3 wd6 Rwc1 wf5")),
                           calcDailyChessScore(Position("Rwd3 wd6 Rwc1 wc5")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Rba5 ba6 Rbb3 bg6")),
                           calcDailyChessScore(Position("Rba5 ba6 Rbb3 bb6")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for only enemy pawns on rook files";

      VERIFY(isBetterScore(calcDailyChessScore(Position("Rwd3 bd6 we4")),
                           calcDailyChessScore(Position("Rwd3 bd6 wd4")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Rbe5 we3 bd6")),
                           calcDailyChessScore(Position("Rbe5 we3 be6")), false),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Bonus for only enemy pawns on "
                                    "rook files for multiple rooks";

      VERIFY(isBetterScore(calcDailyChessScore(Position("Rwd3 bd6 Rwf3 bf2 wc4")),
                           calcDailyChessScore(Position("Rwd3 bd6 Rwf3 bf2 wf4")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Rbh5 wh3 Rbg5 wg3 bb4")),
                           calcDailyChessScore(Position("Rbh5 wh3 Rbg5 wg3 bg4")), false),
             caseLabel);
   }
}

void testDCKnightScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more knights is higher";

      VERIFY(isBetterScore(calcDailyChessScore(Position("Nwe5 Nwd5")),
                           calcDailyChessScore(Position("Nwe5")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Nbe5 Nbe4")),
                           calcDailyChessScore(Position("Nbe5")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for knight closeness to center";

      VERIFY(isBetterScore(calcDailyChessScore(Position("Nwb2")),
                           calcDailyChessScore(Position("Nwa1")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Nwc3")),
                           calcDailyChessScore(Position("Nwb1")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Nwe5")),
                           calcDailyChessScore(Position("Nwc3")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Nbg6")),
                           calcDailyChessScore(Position("Nbh8")), false),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Nbf6")),
                           calcDailyChessScore(Position("Nbg8")), false),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Nbd4")),
                           calcDailyChessScore(Position("Nbe6")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for knight closeness to enemy king";

      // Note - It's a bit tricky to isolate the closeness score from other components of
      //        a knights score, e.g. location on board!
      VERIFY(isBetterScore(calcDailyChessScore(Position("Nwd2 Kbd4")),
                           calcDailyChessScore(Position("Nwc2 Kbd4")), true),
             caseLabel);
      VERIFY(calcDailyChessScore(Position("Nwg8 Kbd4")) ==
                calcDailyChessScore(Position("Nwh7 Kbd4")),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Nbd2 Kwf5")),
                           calcDailyChessScore(Position("Nbc2 Kwf5")), false),
             caseLabel);
      VERIFY(calcDailyChessScore(Position("Nbg8 Kwf5")) ==
                calcDailyChessScore(Position("Nbh7 Kwf5")),
             caseLabel);
   }
}

void testDCQueenScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more queens is higher";

      VERIFY(isBetterScore(calcDailyChessScore(Position("Qwe5 Qwd5")),
                           calcDailyChessScore(Position("Qwe5")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Qbe5 Qbe4")),
                           calcDailyChessScore(Position("Qbe5")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for queen proximity to enemy king";

      VERIFY(isBetterScore(calcDailyChessScore(Position("Kbd5 Qwe5")),
                           calcDailyChessScore(Position("Kbd5 Qwf6")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Kwh8 Qbb2")),
                           calcDailyChessScore(Position("Kwh8 Qba1")), false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for queen and friendly bishop on same diagonal";

      VERIFY(isBetterScore(calcDailyChessScore(Position("Qwe5 Bwc3")),
                           calcDailyChessScore(Position("Qwe5 Bwd3")), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(Position("Qbb7 Bbg2")),
                           calcDailyChessScore(Position("Qbb7 Bbf2")), false),
             caseLabel);
   }
}

void testDCKingScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Penalty for more enemy pieces in king's quadrant";

      // a1 quadrant with more pieces for black.
      VERIFY(isBetterScore(calcDailyChessScore(Position("Kwa2 wb2 Bbd4 Nbg6")),
                           calcDailyChessScore(Position("Kwa2 wb2 Bbd4 Nba4")), true),
             caseLabel);
      // h1 quadrant with more pieces for black.
      VERIFY(isBetterScore(calcDailyChessScore(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg6")),
                           calcDailyChessScore(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3")),
                           true),
             caseLabel);
      // a8 quadrant with more pieces for white.
      VERIFY(isBetterScore(calcDailyChessScore(Position("Kba8 Nba7 Bwc5 wd4")),
                           calcDailyChessScore(Position("Kba8 Nba7 Bwc5 wd5")), false),
             caseLabel);
      // h8 quadrant with more pieces for white.
      VERIFY(isBetterScore(calcDailyChessScore(Position("Kbg8 Nbf6 be7 Bwf5 we5 Rwg4")),
                           calcDailyChessScore(Position("Kbg8 Nbf6 be7 Bwf5 we5 Rwg6")),
                           false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - No penalty for more enemy pieces in king's quadrant when "
         "king is in enemy quadrant";

      // White king in a8 quadrant.
      VERIFY(!isBetterScore(calcDailyChessScore(Position("Kwa5 Bbd7 Nbe5")),
                            calcDailyChessScore(Position("Kwa5 Bbd7 Nbc5")), true),
             caseLabel);
      // Black king in h1 quadrant.
      VERIFY(!isBetterScore(calcDailyChessScore(Position("Kbg2 Nbf3 Bwf4 we4 Rwg5")),
                            calcDailyChessScore(Position("Kbg2 Nbf3 Bwf4 we4 Rwg4")),
                            false),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Higher penalty for more enemy pieces in king's quadrant";

      // White king in h1 quadrant.
      VERIFY(isBetterScore(
                calcDailyChessScore(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3 Rbc3")),
                calcDailyChessScore(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3 Rbe3")), true),
             caseLabel);
      // Black king in a8 quadrant.
      VERIFY(isBetterScore(calcDailyChessScore(Position("Kbg8 Bwf5 Rwg4")),
                           calcDailyChessScore(Position("Kbg8 Bwf5 Rwg6")), false),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Queen counts as more than one "
                                    "piece for in king's quadrant penalty";

      // White king in h1 quadrant.
      VERIFY(isBetterScore(
                calcDailyChessScore(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3 Rbe3")),
                calcDailyChessScore(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3 Qbe3")), true),
             caseLabel);
      // Black king in a8 quadrant.
      VERIFY(isBetterScore(calcDailyChessScore(Position("Kbg8 Bwf5 Rwg6")),
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

      VERIFY(isBetterScore(calcDailyChessScore(hasCastled),
                           calcDailyChessScore(hasNotCastled), true),
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

      VERIFY(isBetterScore(calcDailyChessScore(original),
                           calcDailyChessScore(kingsideRookMoved), true),
             caseLabel);
      VERIFY(isBetterScore(calcDailyChessScore(original),
                           calcDailyChessScore(queensideRookMoved), true),
             caseLabel);
   }
}

void testDailyChessScoring()
{
   testDCPawnScoring();
   testDCBishopScoring();
   testDCRookScoring();
   testDCKnightScoring();
   testDCQueenScoring();
   testDCKingScoring();
}

} // namespace


///////////////////

void testScoring()
{
   testIsBetterScore();
   testGetWorstScore();
   testPieceValueScoring();
   testDailyChessScoring();
}
