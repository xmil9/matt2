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
using namespace pvs;


namespace
{
///////////////////

void testLookupValue()
{
   {
      const std::string caseLabel = "lookupValue returns correct value";

      constexpr PieceValueTable values{1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.};

      VERIFY(lookupValue(Kw, values) == 1., caseLabel);
      VERIFY(lookupValue(Qw, values) == 2., caseLabel);
      VERIFY(lookupValue(Rw, values) == 3., caseLabel);
      VERIFY(lookupValue(Bw, values) == 4., caseLabel);
      VERIFY(lookupValue(Nw, values) == 5., caseLabel);
      VERIFY(lookupValue(Pw, values) == 6., caseLabel);
      VERIFY(lookupValue(Kb, values) == 7., caseLabel);
      VERIFY(lookupValue(Qb, values) == 8., caseLabel);
      VERIFY(lookupValue(Rb, values) == 9., caseLabel);
      VERIFY(lookupValue(Bb, values) == 10., caseLabel);
      VERIFY(lookupValue(Nb, values) == 11., caseLabel);
      VERIFY(lookupValue(Pb, values) == 12., caseLabel);
   }
}

void testCalcPieceValueScoreForPiece()
{
   {
      const std::string caseLabel =
         "score(Position, Piece) for pawns with default piece values";

      VERIFY(score(Position("wa2 wb2"), Pw) > 0., caseLabel);
      VERIFY(score(Position("wa2 wb2"), Pw) > score(Position("wa2"), Pw), caseLabel);
      VERIFY(score(Position("bc7 bd4"), Pb) > 0., caseLabel);
      VERIFY(score(Position("bc7 bd4"), Pb) > score(Position("bd4"), Pb), caseLabel);
   }
   {
      const std::string caseLabel = "score(Position, Piece, "
                                    "PieceValueTable) for pawns with custom piece values";

      constexpr PieceValueTable PieceValues{1., 1., 1., 1., 1., 20.,
                                            1., 1., 1., 1., 1., 30.};

      VERIFY(score(Position("wa2"), Pw, PieceValues) == 20., caseLabel);
      VERIFY(score(Position("wa2 wb2"), Pw, PieceValues) == 40., caseLabel);
      VERIFY(score(Position("ba7"), Pb, PieceValues) == 30., caseLabel);
      VERIFY(score(Position("ba7 bb5"), Pb, PieceValues) == 60., caseLabel);
   }
   {
      const std::string caseLabel =
         "score(Position, Piece) for knights with default piece values";

      VERIFY(score(Position("Nwa2 Nwb2"), Nw) > 0., caseLabel);
      VERIFY(score(Position("Nwa2 Nwb2"), Nw) > score(Position("Nwa2"), Nw), caseLabel);
      VERIFY(score(Position("Nbc7 Nbd4"), Nb) > 0., caseLabel);
      VERIFY(score(Position("Nbc7 Nbd4"), Nb) > score(Position("Nbc7"), Nb), caseLabel);
   }
   {
      const std::string caseLabel =
         "score(Position, Piece, "
         "PieceValueTable) for knights with custom piece values";

      constexpr PieceValueTable PieceValues{1., 1., 1., 1., 20., 1.,
                                            1., 1., 1., 1., 30., 1.};

      VERIFY(score(Position("Nwa2"), Nw, PieceValues) == 20., caseLabel);
      VERIFY(score(Position("Nwa2 Nwb2"), Nw, PieceValues) == 40., caseLabel);
      VERIFY(score(Position("Nba7"), Nb, PieceValues) == 30., caseLabel);
      VERIFY(score(Position("Nba7 Nbb5"), Nb, PieceValues) == 60., caseLabel);
   }
   {
      const std::string caseLabel =
         "score(Position, Piece) for bishops with default piece values";

      VERIFY(score(Position("Bwa2 Bwb2"), Bw) > 0., caseLabel);
      VERIFY(score(Position("Bwa2 Bwb2"), Bw) > score(Position("Bwa2"), Bw), caseLabel);
      VERIFY(score(Position("Bbc7 Bbd4"), Bb) > 0., caseLabel);
      VERIFY(score(Position("Bbc7 Bbd4"), Bb) > score(Position("Bbc7"), Bb), caseLabel);
   }
   {
      const std::string caseLabel =
         "score(Position, Piece, "
         "PieceValueTable) for bishops with custom piece values";

      constexpr PieceValueTable PieceValues{1., 1., 1., 20., 1., 1.,
                                            1., 1., 1., 30., 1., 1.};

      VERIFY(score(Position("Bwa2"), Bw, PieceValues) == 20., caseLabel);
      VERIFY(score(Position("Bwa2 Bwb2"), Bw, PieceValues) == 40., caseLabel);
      VERIFY(score(Position("Bba7"), Bb, PieceValues) == 30., caseLabel);
      VERIFY(score(Position("Bba7 Bbb5"), Bb, PieceValues) == 60., caseLabel);
   }
   {
      const std::string caseLabel =
         "score(Position, Piece) for rook with default piece values";

      VERIFY(score(Position("Rwa2 Rwb2"), Rw) > 0., caseLabel);
      VERIFY(score(Position("Rwa2 Rwb2"), Rw) > score(Position("Rwa2"), Rw), caseLabel);
      VERIFY(score(Position("Rbc7 Rbd4"), Rb) > 0., caseLabel);
      VERIFY(score(Position("Rbc7 Rbd4"), Rb) > score(Position("Rbd4"), Rb), caseLabel);
   }
   {
      const std::string caseLabel = "score(Position, Piece, "
                                    "PieceValueTable) for rook with custom piece values";

      constexpr PieceValueTable PieceValues{1., 1., 20., 1., 1., 1.,
                                            1., 1., 30., 1., 1., 1.};

      VERIFY(score(Position("Rwa2"), Rw, PieceValues) == 20., caseLabel);
      VERIFY(score(Position("Rwa2 Rwb2"), Rw, PieceValues) == 40., caseLabel);
      VERIFY(score(Position("Rba7"), Rb, PieceValues) == 30., caseLabel);
      VERIFY(score(Position("Rba7 Rbb5"), Rb, PieceValues) == 60., caseLabel);
   }
   {
      const std::string caseLabel =
         "score(Position, Piece) for queen with default piece values";

      VERIFY(score(Position("Qwa2 Qwb2"), Qw) > 0., caseLabel);
      VERIFY(score(Position("Qwa2 Qwb2"), Qw) > score(Position("Qwa2"), Qw), caseLabel);
      VERIFY(score(Position("Qbc7 Qbd4"), Qb) > 0., caseLabel);
      VERIFY(score(Position("Qbc7 Qbd4"), Qb) > score(Position("Qbd4"), Qw), caseLabel);
   }
   {
      const std::string caseLabel = "score(Position, Piece, "
                                    "PieceValueTable) for queen with custom piece values";

      constexpr PieceValueTable PieceValues{1., 20., 1., 1., 1., 1.,
                                            1., 30., 1., 1., 1., 1.};

      VERIFY(score(Position("Qwa2"), Qw, PieceValues) == 20., caseLabel);
      VERIFY(score(Position("Qwa2 Qwb2"), Qw, PieceValues) == 40., caseLabel);
      VERIFY(score(Position("Qba7"), Qb, PieceValues) == 30., caseLabel);
      VERIFY(score(Position("Qba7 Qbb5"), Qb, PieceValues) == 60., caseLabel);
   }
   {
      const std::string caseLabel =
         "score(Position, Piece) for king with default piece values";

      VERIFY(score(Position("Kwb2"), Kw) > 0., caseLabel);
      VERIFY(score(Position("Kbc7"), Kb) > 0., caseLabel);
   }
   {
      const std::string caseLabel = "score(Position, Piece, "
                                    "PieceValueTable) for king with custom piece values";

      constexpr PieceValueTable PieceValues{20., 1., 1., 1., 1., 1.,
                                            30., 1., 1., 1., 1., 1.};

      VERIFY(score(Position("Kwa2"), Kw, PieceValues) == 20., caseLabel);
      VERIFY(score(Position("Kba7"), Kb, PieceValues) == 30., caseLabel);
   }
}

void testCalcPieceValueScoreForSide()
{
   {
      const std::string caseLabel = "score(Position, Color) for default piece values";

      VERIFY(score(Position(""), Color::White) == 0., caseLabel);
      VERIFY(score(Position("Kwa2 Qwd1"), Color::White) > 0., caseLabel);
      VERIFY(score(Position("Kwa2 Qwd1"), Color::Black) == 0., caseLabel);
      VERIFY(score(Position("Kba6 be3"), Color::Black) > 0., caseLabel);
      VERIFY(score(Position("Kba6 be3"), Color::White) == 0., caseLabel);
   }
   {
      const std::string caseLabel =
         "score(Position, Color, PieceValueTable) with custom piece values";

      constexpr PieceValueTable PieceValues{10., 9., 8., 7., 6., 5.,
                                            10., 9., 8., 7., 6., 5.};

      VERIFY(score(Position(""), Color::White, PieceValues) == 0., caseLabel);
      VERIFY(score(Position("Kwa2 Qwd1"), Color::White, PieceValues) == 19., caseLabel);
      VERIFY(score(Position("Kba7 be6 Nbf6"), Color::Black, PieceValues) == 21.,
             caseLabel);
   }
}

void testCalcPieceValueScoreForPosition()
{
   {
      const std::string caseLabel = "score(Position) for default piece values";

      VERIFY(score(Position("")) == 0., caseLabel);
      VERIFY(score(Position("Kwa2 Qwd1 Kba6 be3")) > 0., caseLabel);
      VERIFY(score(Position("Kwa2 Kba6 be3")) < 0., caseLabel);
   }
   {
      const std::string caseLabel =
         "score(Position, PieceValueTable) with custom piece values";

      constexpr PieceValueTable PieceValues{10., 9., 8., 7., 6., 5.,
                                            10., 9., 8., 7., 6., 5.};

      VERIFY(score(Position(""), PieceValues) == 0., caseLabel);
      VERIFY(score(Position("Kwa2 Qwd1 Kba6 be3"), PieceValues) == 4., caseLabel);
      VERIFY(score(Position("Kwa2 Kba6 be3"), PieceValues) == -5., caseLabel);
   }
}

} // namespace

///////////////////

void testPieceValueScoring()
{
   testLookupValue();
   testCalcPieceValueScoreForPiece();
   testCalcPieceValueScoreForSide();
   testCalcPieceValueScoreForPosition();
}
