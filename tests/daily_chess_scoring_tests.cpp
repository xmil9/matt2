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

      constexpr Rules rule = Rules::PawnPieceValue;

      VERIFY(bt(score(Position("wa2 wb2"), rule), score(Position("wa2"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("ba7 bb7"), rule), score(Position("ba7"), rule), Black),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Center pawns have higher score";

      constexpr Rules rule = Rules::PawnPositionBonus;

      VERIFY(bt(score(Position("wb3"), rule), score(Position("wa3"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("wc3"), rule), score(Position("wb3"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("wd3"), rule), score(Position("wc3"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("we3"), rule), score(Position("wf3"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("wf3"), rule), score(Position("wg3"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("wg3"), rule), score(Position("wh3"), rule), White),
             caseLabel);

      VERIFY(bt(score(Position("bb5"), rule), score(Position("ba5"), rule), Black),
             caseLabel);
      VERIFY(bt(score(Position("bc5"), rule), score(Position("bb5"), rule), Black),
             caseLabel);
      VERIFY(bt(score(Position("bd5"), rule), score(Position("bc5"), rule), Black),
             caseLabel);
      VERIFY(bt(score(Position("be5"), rule), score(Position("bf5"), rule), Black),
             caseLabel);
      VERIFY(bt(score(Position("bf5"), rule), score(Position("bg5"), rule), Black),
             caseLabel);
      VERIFY(bt(score(Position("bg5"), rule), score(Position("bh5"), rule), Black),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Pawns on deeper ranks have higher score";

      constexpr Rules rule = Rules::PawnPositionBonus;

      VERIFY(bt(score(Position("wb3"), rule), score(Position("wb2"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("wc4"), rule), score(Position("wc3"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("wd5"), rule), score(Position("wd4"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("we6"), rule), score(Position("we5"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("wf7"), rule), score(Position("wf6"), rule), White),
             caseLabel);

      VERIFY(bt(score(Position("bg6"), rule), score(Position("bg7"), rule), Black),
             caseLabel);
      VERIFY(bt(score(Position("bf5"), rule), score(Position("bf6"), rule), Black),
             caseLabel);
      VERIFY(bt(score(Position("be4"), rule), score(Position("be5"), rule), Black),
             caseLabel);
      VERIFY(bt(score(Position("bd3"), rule), score(Position("bd4"), rule), Black),
             caseLabel);
      VERIFY(bt(score(Position("bc2"), rule), score(Position("bc3"), rule), Black),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Passed pawns have higher score - White";

      constexpr Rules rule = Rules::PassedPawnBonus;

      const Position passed("wd5 bf7");
      const Position blocked("wd5 bd7");
      const Position neighborA("wd5 bc7");
      const Position neighborB("wd5 be7");

      VERIFY(cmp(score(passed, rule), 0., White) == 1, caseLabel);
      // No bonus because pawn is not passed.
      VERIFY(cmp(score(blocked, rule), 0., White) == 0, caseLabel);
      VERIFY(cmp(score(neighborA, rule), 0., White) == 0, caseLabel);
      VERIFY(cmp(score(neighborB, rule), 0., White) == 0, caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Passed pawns have higher score - Black";

      constexpr Rules rule = Rules::PassedPawnBonus;

      const Position passed("bf3 wc2");
      const Position blocked("bf3 wf2");
      const Position neighborA("bf3 we2");
      const Position neighborB("bf3 wg2");

      VERIFY(cmp(score(passed, rule), 0., Black) == 1, caseLabel);
      // No bonus because pawn is not passed.
      VERIFY(cmp(score(blocked, rule), 0., Black) == 0, caseLabel);
      VERIFY(cmp(score(neighborA, rule), 0., Black) == 0, caseLabel);
      VERIFY(cmp(score(neighborB, rule), 0., Black) == 0, caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Double pawn penalty - White";

      constexpr Rules rule = Rules::DoublePawnPenalty;
      VERIFY(cmp(score(Position("wg4 wg5"), rule), 0., White) == -1, caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Double pawn penalty - Black";

      constexpr Rules rule = Rules::DoublePawnPenalty;
      VERIFY(cmp(score(Position("be3 be5"), rule), 0., Black) == -1, caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Isolated pawn penalty - White";

      constexpr Rules rule = Rules::IsolatedPawnPenalty;
      VERIFY(cmp(score(Position("wg4 wc5"), rule), 0., White) == -1, caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Isolated pawn penalty - Black";

      constexpr Rules rule = Rules::IsolatedPawnPenalty;
      VERIFY(cmp(score(Position("bc6 be7"), rule), 0., Black) == -1, caseLabel);
   }
}

void testBishopScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more bishops is higher";

      constexpr Rules rule = Rules::BishopPieceValue;

      VERIFY(bt(score(Position("Bwe5 Bwd5"), rule), score(Position("Bwe5"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("Bbe5 Bbe4"), rule), score(Position("Bbe5"), rule), Black),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Multiple bishops receive a bonus";

      constexpr Rules rule = Rules::MultipleBishopBonus;
      VERIFY(cmp(score(Position("Bwe5 Bwd5"), rule), 0., White) == 1, caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Penality if own pawn is diagonal neighbor of bishop";

      constexpr Rules rule = Rules::BishopAdjacentPawnPenality;
      VERIFY(cmp(score(Position("Bwe5 wd4"), rule), 0., White) == -1, caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Penality if opposing pawn is diagonal neighbor of bishop";

      constexpr Rules rule = Rules::BishopAdjacentPawnPenality;
      VERIFY(cmp(score(Position("Bbf3 wg4"), rule), 0., Black) == -1, caseLabel);
   }
}

void testRookScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more rooks is higher";

      constexpr Rules rule = Rules::RookPieceValue;
      VERIFY(bt(score(Position("Rwe5 Rwd5"), rule), score(Position("Rwe5"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("Rbe5 Rbe4"), rule), score(Position("Rbe5"), rule), Black),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for rook proximity to enemy king";

      constexpr Rules rule = Rules::RookKingClosenessBonus;
      VERIFY(bt(score(Position("Kbd5 Rwe5"), rule), score(Position("Kbd5 Rwf6"), rule),
                White),
             caseLabel);
      VERIFY(bt(score(Position("Kwh8 Rbb2"), rule), score(Position("Kwh8 Rba1"), rule),
                Black),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Bonus for rook on 7th rank";

      constexpr Rules rule = Rules::RookSeventhRankBonus;
      VERIFY(cmp(score(Position("Rwd7"), rule), 0., White) == 1, caseLabel);
      VERIFY(cmp(score(Position("Rbd2"), rule), 0., Black) == 1, caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for rooks on shared file";

      constexpr Rules rule = Rules::RookSharedFileBonus;
      VERIFY(cmp(score(Position("Rwd3 Rwd6"), rule), 0., White) == 1, caseLabel);
      VERIFY(cmp(score(Position("Rbg5 Rbg1"), rule), 0., Black) == 1, caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for no pawns on rook files";

      constexpr Rules rule = Rules::RookPawnsOnFileBonus;
      VERIFY(
         bt(score(Position("Rwd3 we6"), rule), score(Position("Rwd3 wd6"), rule), White),
         caseLabel);
      VERIFY(
         bt(score(Position("Rbe5 bd3"), rule), score(Position("Rbe5 be4"), rule), Black),
         caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for no pawns on rook files for multiple rooks";

      constexpr Rules rule = Rules::RookPawnsOnFileBonus;
      VERIFY(bt(score(Position("Rwd3 wd6 Rwc1 wf5"), rule),
                score(Position("Rwd3 wd6 Rwc1 wc5"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("Rba5 ba6 Rbb3 bg6"), rule),
                score(Position("Rba5 ba6 Rbb3 bb6"), rule), Black),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for only enemy pawns on rook files";

      constexpr Rules rule = Rules::RookPawnsOnFileBonus;
      VERIFY(bt(score(Position("Rwd3 bd6 we4"), rule),
                score(Position("Rwd3 bd6 wd4"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("Rbe5 we3 bd6"), rule),
                score(Position("Rbe5 we3 be6"), rule), Black),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Bonus for only enemy pawns on "
                                    "rook files for multiple rooks";

      constexpr Rules rule = Rules::RookPawnsOnFileBonus;
      VERIFY(bt(score(Position("Rwd3 bd6 Rwf3 bf2 wc4"), rule),
                score(Position("Rwd3 bd6 Rwf3 bf2 wf4"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("Rbh5 wh3 Rbg5 wg3 bb4"), rule),
                score(Position("Rbh5 wh3 Rbg5 wg3 bg4"), rule), Black),
             caseLabel);
   }
}

void testKnightScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more knights is higher";

      constexpr Rules rule = Rules::KnightPieceValue;
      VERIFY(bt(score(Position("Nwe5 Nwd5"), rule), score(Position("Nwe5"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("Nbe5 Nbe4"), rule), score(Position("Nbe5"), rule), Black),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for knight closeness to center";

      constexpr Rules rule = Rules::KnightCenterBonus;
      VERIFY(bt(score(Position("Nwb2"), rule), score(Position("Nwa1"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("Nwc3"), rule), score(Position("Nwb1"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("Nwe5"), rule), score(Position("Nwc3"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("Nbg6"), rule), score(Position("Nbh8"), rule), Black),
             caseLabel);
      VERIFY(bt(score(Position("Nbf6"), rule), score(Position("Nbg8"), rule), Black),
             caseLabel);
      VERIFY(bt(score(Position("Nbd4"), rule), score(Position("Nbe6"), rule), Black),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for knight closeness to enemy king";

      constexpr Rules rule = Rules::KnightKingClosenessBonus;

      VERIFY(bt(score(Position("Nwd2 Kbd4"), rule), score(Position("Nwc2 Kbd4"), rule),
                White),
             caseLabel);
      VERIFY(score(Position("Nwg8 Kbd4"), rule) == score(Position("Nwh7 Kbd4"), rule),
             caseLabel);
      VERIFY(bt(score(Position("Nbd2 Kwf5"), rule), score(Position("Nbc2 Kwf5"), rule),
                Black),
             caseLabel);
      VERIFY(score(Position("Nbg8 Kwf5"), rule) == score(Position("Nbh7 Kwf5"), rule),
             caseLabel);
   }
}

void testQueenScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Score for more queens is higher";

      constexpr Rules rule = Rules::QueenPieceValue;
      VERIFY(bt(score(Position("Qwe5 Qwd5"), rule), score(Position("Qwe5"), rule), White),
             caseLabel);
      VERIFY(bt(score(Position("Qbe5 Qbe4"), rule), score(Position("Qbe5"), rule), Black),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for queen proximity to enemy king";

      constexpr Rules rule = Rules::QueenKingClosenessValue;
      VERIFY(bt(score(Position("Kbd5 Qwe5"), rule), score(Position("Kbd5 Qwf6"), rule),
                White),
             caseLabel);
      VERIFY(bt(score(Position("Kwh8 Qbb2"), rule), score(Position("Kwh8 Qba1"), rule),
                Black),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Bonus for queen and friendly bishop on same diagonal";

      constexpr Rules rule = Rules::QueenBishopDiagonalClosenessValue;
      VERIFY(bt(score(Position("Qwe5 Bwc3"), rule), score(Position("Qwe5 Bwd3"), rule),
                White),
             caseLabel);
      VERIFY(bt(score(Position("Qbb7 Bbg2"), rule), score(Position("Qbb7 Bbf2"), rule),
                Black),
             caseLabel);
   }
}

void testKingScoring()
{
   {
      const std::string caseLabel =
         "Daily chess scoring - Penalty for more enemy pieces in king's quadrant";

      constexpr Rules rule = Rules::KingQuadrantPenalty;

      // a1 quadrant with more pieces for black.
      VERIFY(cmp(score(Position("Kwa2 wb2 Bbd4 Nba4"), rule), 0., White) == -1,
             caseLabel);
      // h1 quadrant with more pieces for black.
      VERIFY(cmp(score(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3"), rule), 0., White) == -1,
             caseLabel);
      // a8 quadrant with more pieces for white.
      VERIFY(cmp(score(Position("Kba8 Nba7 Bwc5 wd5"), rule), 0., Black) == -1,
             caseLabel);
      // h8 quadrant with more pieces for white.
      VERIFY(cmp(score(Position("Kbg8 Nbf6 be7 Bwf5 we5 Rwg6"), rule), 0., Black) == -1,
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - No penalty for more enemy pieces in king's quadrant when "
         "king is in enemy quadrant";

      constexpr Rules rule = Rules::KingQuadrantPenalty;

      // White king in a8 quadrant.
      VERIFY(cmp(score(Position("Kwa5 Bbd7 Nbe5"), rule), 0., White) == 0, caseLabel);
      // Black king in h1 quadrant.
      VERIFY(cmp(score(Position("Kbg2 Nbf3 Bwf4 we4 Rwg4"), rule), 0., Black) == 0,
             caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Higher penalty for more enemy pieces in king's quadrant";

      constexpr Rules rule = Rules::KingQuadrantPenalty;

      // White king in h1 quadrant.
      VERIFY(bt(score(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3"), rule),
                score(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3 Rbe3"), rule), White),
             caseLabel);
      // Black king in a8 quadrant.
      VERIFY(bt(score(Position("Kbg8 Bwf5"), rule),
                score(Position("Kbg8 Bwf5 Rwg6"), rule), Black),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Queen counts as more than one "
                                    "piece for in king's quadrant penalty";

      constexpr Rules rule = Rules::KingQuadrantPenalty;

      // White king in h1 quadrant.
      VERIFY(bt(score(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3 Rbe3"), rule),
                score(Position("Kwg2 wh2 Rwh1 Bbe4 Rbh4 Nbg3 Qbe3"), rule), White),
             caseLabel);
      // Black king in a8 quadrant.
      VERIFY(bt(score(Position("Kbg8 Bwf5 Rwg6"), rule),
                score(Position("Kbg8 Bwf5 Qwg6"), rule), Black),
             caseLabel);
   }
   {
      const std::string caseLabel = "Daily chess scoring - Never castled penalty";

      constexpr Rules rule = Rules::KingCastlingPenalty;

      Position hasNotCastled("Kwe2 Rwa1 Rwh1");
      // No castling possible anymore.
      hasNotCastled.hasKingMoved(White);
      // Set black to have castled to prevent it from also getting a penality.
      hasNotCastled.setHasCastled(Black);

      VERIFY(cmp(score(hasNotCastled, rule), 0., White) == -1, caseLabel);
   }
   {
      const std::string caseLabel =
         "Daily chess scoring - Penality when castling possible but rook moved";

      constexpr Rules rule = Rules::KingCastlingPenalty;

      Position kingsideRookMoved("Kwe1 Rwa1 Rwg1");
      kingsideRookMoved.hasRookMoved(White, true);
      // Set black to have castled to prevent it from also getting a penality.
      kingsideRookMoved.setHasCastled(Black);

      Position queensideRookMoved("Kwe1 Rwb1 Rwh1");
      queensideRookMoved.hasRookMoved(White, false);
      // Set black to have castled to prevent it from also getting a penality.
      queensideRookMoved.setHasCastled(Black);

      VERIFY(cmp(score(kingsideRookMoved, rule), 0., White) == -1, caseLabel);
      VERIFY(cmp(score(queensideRookMoved, rule), 0., White) == -1, caseLabel);
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
