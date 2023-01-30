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
         "calcPieceValueScore(Position, Piece) for pawns with default piece values";

      VERIFY(calcPieceValueScore(Position("wa2 wb2"), Pw) > 0., caseLabel);
      VERIFY(calcPieceValueScore(Position("wa2 wb2"), Pw) >
                calcPieceValueScore(Position("wa2"), Pw),
             caseLabel);
      VERIFY(calcPieceValueScore(Position("bc7 bd4"), Pb) > 0., caseLabel);
      VERIFY(calcPieceValueScore(Position("bc7 bd4"), Pb) >
                calcPieceValueScore(Position("bd4"), Pb),
             caseLabel);
   }
   {
      const std::string caseLabel = "calcPieceValueScore(Position, Piece, "
                                    "PieceValueTable) for pawns with custom piece values";

      constexpr PieceValueTable PieceValues{1., 1., 1., 1., 1., 20.,
                                            1., 1., 1., 1., 1., 30.};

      VERIFY(calcPieceValueScore(Position("wa2"), Pw, PieceValues) == 20., caseLabel);
      VERIFY(calcPieceValueScore(Position("wa2 wb2"), Pw, PieceValues) == 40., caseLabel);
      VERIFY(calcPieceValueScore(Position("ba7"), Pb, PieceValues) == 30., caseLabel);
      VERIFY(calcPieceValueScore(Position("ba7 bb5"), Pb, PieceValues) == 60., caseLabel);
   }
   {
      const std::string caseLabel =
         "calcPieceValueScore(Position, Piece) for knights with default piece values";

      VERIFY(calcPieceValueScore(Position("Nwa2 Nwb2"), Nw) > 0., caseLabel);
      VERIFY(calcPieceValueScore(Position("Nwa2 Nwb2"), Nw) >
                calcPieceValueScore(Position("Nwa2"), Nw),
             caseLabel);
      VERIFY(calcPieceValueScore(Position("Nbc7 Nbd4"), Nb) > 0., caseLabel);
      VERIFY(calcPieceValueScore(Position("Nbc7 Nbd4"), Nb) >
                calcPieceValueScore(Position("Nbc7"), Nb),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "calcPieceValueScore(Position, Piece, "
         "PieceValueTable) for knights with custom piece values";

      constexpr PieceValueTable PieceValues{1., 1., 1., 1., 20., 1.,
                                            1., 1., 1., 1., 30., 1.};

      VERIFY(calcPieceValueScore(Position("Nwa2"), Nw, PieceValues) == 20., caseLabel);
      VERIFY(calcPieceValueScore(Position("Nwa2 Nwb2"), Nw, PieceValues) == 40.,
             caseLabel);
      VERIFY(calcPieceValueScore(Position("Nba7"), Nb, PieceValues) == 30., caseLabel);
      VERIFY(calcPieceValueScore(Position("Nba7 Nbb5"), Nb, PieceValues) == 60.,
             caseLabel);
   }
   {
      const std::string caseLabel =
         "calcPieceValueScore(Position, Piece) for bishops with default piece values";

      VERIFY(calcPieceValueScore(Position("Bwa2 Bwb2"), Bw) > 0., caseLabel);
      VERIFY(calcPieceValueScore(Position("Bwa2 Bwb2"), Bw) >
                calcPieceValueScore(Position("Bwa2"), Bw),
             caseLabel);
      VERIFY(calcPieceValueScore(Position("Bbc7 Bbd4"), Bb) > 0., caseLabel);
      VERIFY(calcPieceValueScore(Position("Bbc7 Bbd4"), Bb) >
                calcPieceValueScore(Position("Bbc7"), Bb),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "calcPieceValueScore(Position, Piece, "
         "PieceValueTable) for bishops with custom piece values";

      constexpr PieceValueTable PieceValues{1., 1., 1., 20., 1., 1.,
                                            1., 1., 1., 30., 1., 1.};

      VERIFY(calcPieceValueScore(Position("Bwa2"), Bw, PieceValues) == 20., caseLabel);
      VERIFY(calcPieceValueScore(Position("Bwa2 Bwb2"), Bw, PieceValues) == 40.,
             caseLabel);
      VERIFY(calcPieceValueScore(Position("Bba7"), Bb, PieceValues) == 30., caseLabel);
      VERIFY(calcPieceValueScore(Position("Bba7 Bbb5"), Bb, PieceValues) == 60.,
             caseLabel);
   }
   {
      const std::string caseLabel =
         "calcPieceValueScore(Position, Piece) for rook with default piece values";

      VERIFY(calcPieceValueScore(Position("Rwa2 Rwb2"), Rw) > 0., caseLabel);
      VERIFY(calcPieceValueScore(Position("Rwa2 Rwb2"), Rw) >
                calcPieceValueScore(Position("Rwa2"), Rw),
             caseLabel);
      VERIFY(calcPieceValueScore(Position("Rbc7 Rbd4"), Rb) > 0., caseLabel);
      VERIFY(calcPieceValueScore(Position("Rbc7 Rbd4"), Rb) >
                calcPieceValueScore(Position("Rbd4"), Rb),
             caseLabel);
   }
   {
      const std::string caseLabel = "calcPieceValueScore(Position, Piece, "
                                    "PieceValueTable) for rook with custom piece values";

      constexpr PieceValueTable PieceValues{1., 1., 20., 1., 1., 1.,
                                            1., 1., 30., 1., 1., 1.};

      VERIFY(calcPieceValueScore(Position("Rwa2"), Rw, PieceValues) == 20., caseLabel);
      VERIFY(calcPieceValueScore(Position("Rwa2 Rwb2"), Rw, PieceValues) == 40.,
             caseLabel);
      VERIFY(calcPieceValueScore(Position("Rba7"), Rb, PieceValues) == 30., caseLabel);
      VERIFY(calcPieceValueScore(Position("Rba7 Rbb5"), Rb, PieceValues) == 60.,
             caseLabel);
   }
   {
      const std::string caseLabel =
         "calcPieceValueScore(Position, Piece) for queen with default piece values";

      VERIFY(calcPieceValueScore(Position("Qwa2 Qwb2"), Qw) > 0., caseLabel);
      VERIFY(calcPieceValueScore(Position("Qwa2 Qwb2"), Qw) >
                calcPieceValueScore(Position("Qwa2"), Qw),
             caseLabel);
      VERIFY(calcPieceValueScore(Position("Qbc7 Qbd4"), Qb) > 0., caseLabel);
      VERIFY(calcPieceValueScore(Position("Qbc7 Qbd4"), Qb) >
                calcPieceValueScore(Position("Qbd4"), Qw),
             caseLabel);
   }
   {
      const std::string caseLabel = "calcPieceValueScore(Position, Piece, "
                                    "PieceValueTable) for queen with custom piece values";

      constexpr PieceValueTable PieceValues{1., 20., 1., 1., 1., 1.,
                                            1., 30., 1., 1., 1., 1.};

      VERIFY(calcPieceValueScore(Position("Qwa2"), Qw, PieceValues) == 20., caseLabel);
      VERIFY(calcPieceValueScore(Position("Qwa2 Qwb2"), Qw, PieceValues) == 40.,
             caseLabel);
      VERIFY(calcPieceValueScore(Position("Qba7"), Qb, PieceValues) == 30., caseLabel);
      VERIFY(calcPieceValueScore(Position("Qba7 Qbb5"), Qb, PieceValues) == 60.,
             caseLabel);
   }
   {
      const std::string caseLabel =
         "calcPieceValueScore(Position, Piece) for king with default piece values";

      VERIFY(calcPieceValueScore(Position("Kwb2"), Kw) > 0., caseLabel);
      VERIFY(calcPieceValueScore(Position("Kbc7"), Kb) > 0., caseLabel);
   }
   {
      const std::string caseLabel = "calcPieceValueScore(Position, Piece, "
                                    "PieceValueTable) for king with custom piece values";

      constexpr PieceValueTable PieceValues{20., 1., 1., 1., 1., 1.,
                                            30., 1., 1., 1., 1., 1.};

      VERIFY(calcPieceValueScore(Position("Kwa2"), Kw, PieceValues) == 20., caseLabel);
      VERIFY(calcPieceValueScore(Position("Kba7"), Kb, PieceValues) == 30., caseLabel);
   }
}

} // namespace

///////////////////

void testPieceValueScoring()
{
   testLookupValue();
   testCalcPieceValueScoreForPiece();
}
