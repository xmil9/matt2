//
// Jan-2023, Michael Lindner
// MIT license
//
#include "piece_value_scoring_tests.h"
#include "piece_value_scoring.h"
#include "position.h"
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
         "Piece value scoring - Score for more pawns is higher";

      VERIFY(calcPieceValueScore(Position("wa2 wb2")) >
                calcPieceValueScore(Position("wa2")),
             caseLabel);
   }
}

void testKnightScoring()
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

void testBishopScoring()
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

void testRookScoring()
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

void testQueenScoring()
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

void testKingScoring()
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

} // namespace

///////////////////

void testPieceValueScoring()
{
   testPawnScoring();
   testKnightScoring();
   testBishopScoring();
   testRookScoring();
   testQueenScoring();
   testKingScoring();
}
