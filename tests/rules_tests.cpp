//
// Jul-2021, Michael Lindner
// MIT license
//
#include "rules_tests.h"
#include "rules.h"
#include "test_util.h"
#include <algorithm>
#include <stdexcept>

using namespace matt2;


namespace
{
///////////////////

bool contains(const std::vector<Move>& moves, const Move& m)
{
   return std::find(std::begin(moves), std::end(moves), m) != std::end(moves);
}


bool contains(const std::vector<Square>& loc, Square sq)
{
   return std::find(std::begin(loc), std::end(loc), sq) != std::end(loc);
}


///////////////////

void testCollectKingMoves()
{
   {
      const std::string caseLabel = "collectKingMoves for all possible moves";

      std::vector<Move> moves;
      collectKingMoves(Kb, c5, Position{"Kbc5"}, moves);

      VERIFY(moves.size() == 8, caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, b4))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, b5))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, b6))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, c4))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, c6))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, d4))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, d5))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, d6))), caseLabel);
   }
   {
      const std::string caseLabel = "collectKingMoves on side of board";

      std::vector<Move> moves;
      collectKingMoves(Kb, a4, Position{"Kba4"}, moves);

      VERIFY(moves.size() == 5, caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, a4, a3))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, a4, a5))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, a4, b3))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, a4, b4))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, a4, b5))), caseLabel);
   }
   {
      const std::string caseLabel = "collectKingMoves in corner of board";

      std::vector<Move> moves;
      collectKingMoves(Kb, h8, Position{"Kbh8"}, moves);

      VERIFY(moves.size() == 3, caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, h8, g8))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, h8, g7))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, h8, h7))), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectKingMoves with neighboring pieces of same color";

      std::vector<Move> moves;
      collectKingMoves(Kb, c5, Position{"Kbc5 bb4 Rbd5 Nbc6"}, moves);

      VERIFY(moves.size() == 5, caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, b5))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, b6))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, c4))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, d4))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, d6))), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectKingMoves with neighboring pieces of opposite color";

      std::vector<Move> moves;
      collectKingMoves(Kb, c5, Position{"Kbc5 wb4 Rwd5 Nwc6"}, moves);

      VERIFY(moves.size() == 8, caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, b4), Pw)), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, b5))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, b6))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, c4))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, c6), Nw)), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, d4))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, d5), Rw)), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Kb, c5, d6))), caseLabel);
   }
}


void testCollectQueenMoves()
{
   {
      const std::string caseLabel = "collectQueenMoves for all moves at d4";

      std::vector<Move> moves;
      collectQueenMoves(Qw, d4, Position{"Qwd4"}, moves);

      const std::vector<Square> destinations = {c5, b6, a7, e3, f2, g1, d5, d6, d7,
                                                d8, d3, d2, d1, e5, f6, g7, h8, c3,
                                                b2, a1, c4, b4, a4, e4, f4, g4, h4};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Qw, d4, to))), caseLabel);
   }
   {
      const std::string caseLabel = "collectQueenMoves for all moves at g6";

      std::vector<Move> moves;
      collectQueenMoves(Qb, g6, Position{"Qbg6"}, moves);

      const std::vector<Square> destinations = {f7, e8, h5, g5, g4, g3, g2, g1,
                                                g7, g8, h7, f5, e4, d3, c2, b1,
                                                f6, e6, d6, c6, b6, a6, h6};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Qb, g6, to))), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectQueenMoves for all moves at a3 (side of board)";

      std::vector<Move> moves;
      collectQueenMoves(Qb, a3, Position{"Qba3"}, moves);

      const std::vector<Square> destinations = {b2, c1, a4, a5, a6, a7, a8,
                                                a1, a2, b4, c5, d6, e7, f8,
                                                b3, c3, d3, e3, f3, g3, h3};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Qb, a3, to))), caseLabel);
   }
   {
      const std::string caseLabel = "collectQueenMoves for all moves at h1 (corner)";

      std::vector<Move> moves;
      collectQueenMoves(Qw, h1, Position{"Qwh1"}, moves);

      const std::vector<Square> destinations = {g2, f3, e4, d5, c6, b7, a8,
                                                h2, h3, h4, h5, h6, h7, h8,
                                                g1, f1, e1, d1, c1, b1, a1};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Qw, h1, to))), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectQueenMoves with diagonal pieces of same color";

      std::vector<Move> moves;
      collectQueenMoves(Qw, d4, Position{"Qwd4 wf6 Rwe3"}, moves);

      const std::vector<Square> destinations = {c5, b6, a7, d5, d6, d7, d8,
                                                d3, d2, d1, e5, c3, b2, a1,
                                                c4, b4, a4, e4, f4, g4, h4};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Qw, d4, to))), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectQueenMoves with horiz and vert pieces of same color";

      std::vector<Move> moves;
      collectQueenMoves(Qw, d4, Position{"Qwd4 wd6 Bwb4"}, moves);

      const std::vector<Square> destinations = {c5, b6, a7, e3, f2, g1, d5, d3,
                                                d2, d1, e5, f6, g7, h8, c3, b2,
                                                a1, c4, e4, f4, g4, h4};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Qw, d4, to))), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectQueenMoves with diagonal pieces of opposite color";

      std::vector<Move> moves;
      collectQueenMoves(Qw, d4, Position{"Qwd4 bf6 Rbe3"}, moves);

      const std::vector<Square> destinations = {c5, b6, a7, d5, d6, d7, d8,
                                                d3, d2, d1, e5, c3, b2, a1,
                                                c4, b4, a4, e4, f4, g4, h4};
      std::vector<Move> expected;
      std::transform(std::begin(destinations), std::end(destinations),
                     std::back_inserter(expected),
                     [](Square to) { return BasicMove(Relocation(Qw, d4, to)); });
      expected.push_back(BasicMove(Relocation(Qw, d4, f6), Pb));
      expected.push_back(BasicMove(Relocation(Qw, d4, e3), Rb));

      VERIFY(moves.size() == expected.size(), caseLabel);
      for (const auto& m : expected)
         VERIFY(contains(moves, m), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectQueenMoves with horiz and vert pieces of opposite color";

      std::vector<Move> moves;
      collectQueenMoves(Qw, d4, Position{"Qwd4 bd6 Bbb4"}, moves);

      const std::vector<Square> destinations = {c5, b6, a7, e3, f2, g1, d5, d3,
                                                d2, d1, e5, f6, g7, h8, c3, b2,
                                                a1, c4, e4, f4, g4, h4};
      std::vector<Move> expected;
      std::transform(std::begin(destinations), std::end(destinations),
                     std::back_inserter(expected),
                     [](Square to) { return BasicMove(Relocation(Qw, d4, to)); });
      expected.push_back(BasicMove(Relocation(Qw, d4, d6), Pb));
      expected.push_back(BasicMove(Relocation(Qw, d4, b4), Bb));

      VERIFY(moves.size() == expected.size(), caseLabel);
      for (const auto& m : expected)
         VERIFY(contains(moves, m), caseLabel);
   }
}


void testCollectRookMoves()
{
   {
      const std::string caseLabel = "collectRookMoves for all moves at d4";

      std::vector<Move> moves;
      collectRookMoves(Rw, d4, Position{"Rwd4"}, moves);

      const std::vector<Square> destinations = {d5, d6, d7, d8, d3, d2, d1,
                                                c4, b4, a4, e4, f4, g4, h4};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Rw, d4, to))), caseLabel);
   }
   {
      const std::string caseLabel = "collectRookMoves for all moves at g6";

      std::vector<Move> moves;
      collectRookMoves(Rb, g6, Position{"Rbg6"}, moves);

      const std::vector<Square> destinations = {g5, g4, g3, g2, g1, g7, g8,
                                                f6, e6, d6, c6, b6, a6, h6};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Rb, g6, to))), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectRookMoves for all moves at a3 (side of board)";

      std::vector<Move> moves;
      collectRookMoves(Rb, a3, Position{"Rba3"}, moves);

      const std::vector<Square> destinations = {a4, a5, a6, a7, a8, a1, a2,
                                                b3, c3, d3, e3, f3, g3, h3};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Rb, a3, to))), caseLabel);
   }
   {
      const std::string caseLabel = "collectRookMoves for all moves at h1 (corner)";

      std::vector<Move> moves;
      collectRookMoves(Rw, h1, Position{"Rwh1"}, moves);

      const std::vector<Square> destinations = {h2, h3, h4, h5, h6, h7, h8,
                                                g1, f1, e1, d1, c1, b1, a1};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Rw, h1, to))), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectRookMoves with horiz and vert pieces of same color";

      std::vector<Move> moves;
      collectRookMoves(Rw, d4, Position{"Rwd4 wd6 Bwb4"}, moves);

      const std::vector<Square> destinations = {d5, d3, d2, d1, c4, e4, f4, g4, h4};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Rw, d4, to))), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectRookMoves with horiz and vert pieces of opposite color";

      std::vector<Move> moves;
      collectRookMoves(Rw, d4, Position{"Rwd4 bd6 Bbb4"}, moves);

      const std::vector<Square> destinations = {d5, d3, d2, d1, c4, e4, f4, g4, h4};
      std::vector<Move> expected;
      std::transform(std::begin(destinations), std::end(destinations),
                     std::back_inserter(expected),
                     [](Square to) { return BasicMove(Relocation(Rw, d4, to)); });
      expected.push_back(BasicMove(Relocation(Rw, d4, d6), Pb));
      expected.push_back(BasicMove(Relocation(Rw, d4, b4), Bb));

      VERIFY(moves.size() == expected.size(), caseLabel);
      for (const auto& m : expected)
         VERIFY(contains(moves, m), caseLabel);
   }
}


void testCollectBishopMoves()
{
   {
      const std::string caseLabel = "collectBishopMoves for all moves at d4";

      std::vector<Move> moves;
      collectBishopMoves(Bw, d4, Position{"Bwd4"}, moves);

      const std::vector<Square> destinations = {c5, b6, a7, e3, f2, g1, e5,
                                                f6, g7, h8, c3, b2, a1};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Bw, d4, to))), caseLabel);
   }
   {
      const std::string caseLabel = "collectBishopMoves for all moves at g6";

      std::vector<Move> moves;
      collectBishopMoves(Bb, g6, Position{"Bbg6"}, moves);

      const std::vector<Square> destinations = {f7, e8, h5, h7, f5, e4, d3, c2, b1};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Bb, g6, to))), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectBishopMoves for all moves at a3 (side of board)";

      std::vector<Move> moves;
      collectBishopMoves(Bb, a3, Position{"Bba3"}, moves);

      const std::vector<Square> destinations = {b2, c1, b4, c5, d6, e7, f8};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Bb, a3, to))), caseLabel);
   }
   {
      const std::string caseLabel = "collectBishopMoves for all moves at h1 (corner)";

      std::vector<Move> moves;
      collectBishopMoves(Bw, h1, Position{"Bwh1"}, moves);

      const std::vector<Square> destinations = {g2, f3, e4, d5, c6, b7, a8};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Bw, h1, to))), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectBishopMoves with diagonal pieces of same color";

      std::vector<Move> moves;
      collectBishopMoves(Bw, d4, Position{"Bwd4 wf6 Rwe3"}, moves);

      const std::vector<Square> destinations = {c5, b6, a7, e5, c3, b2, a1};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Bw, d4, to))), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectBishopMoves with diagonal pieces of opposite color";

      std::vector<Move> moves;
      collectBishopMoves(Bw, d4, Position{"Bwd4 bf6 Rbe3"}, moves);

      const std::vector<Square> destinations = {c5, b6, a7, e5, c3, b2, a1};
      std::vector<Move> expected;
      std::transform(std::begin(destinations), std::end(destinations),
                     std::back_inserter(expected),
                     [](Square to) { return BasicMove(Relocation(Bw, d4, to)); });
      expected.push_back(BasicMove(Relocation(Bw, d4, f6), Pb));
      expected.push_back(BasicMove(Relocation(Bw, d4, e3), Rb));

      VERIFY(moves.size() == expected.size(), caseLabel);
      for (const auto& m : expected)
         VERIFY(contains(moves, m), caseLabel);
   }
}


void testCollectKnightMoves()
{
   {
      const std::string caseLabel = "collectKnightMoves for all moves at d4";

      std::vector<Move> moves;
      collectKnightMoves(Nw, d4, Position{"Nwd4"}, moves);

      const std::vector<Square> destinations = {c6, e6, c2, e2, b3, b5, f3, f5};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Nw, d4, to))), caseLabel);
   }
   {
      const std::string caseLabel = "collectKnightMoves for all moves at g6";

      std::vector<Move> moves;
      collectKnightMoves(Nb, g6, Position{"Nbg6"}, moves);

      const std::vector<Square> destinations = {f4, h4, f8, h8, e5, e7};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Nb, g6, to))), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectKnightMoves for all moves at a3 (side of board)";

      std::vector<Move> moves;
      collectKnightMoves(Nb, a3, Position{"Nba3"}, moves);

      const std::vector<Square> destinations = {b5, b1, c4, c2};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Nb, a3, to))), caseLabel);
   }
   {
      const std::string caseLabel = "collectKnightMoves for all moves at h1 (corner)";

      std::vector<Move> moves;
      collectKnightMoves(Nw, h1, Position{"Nwh1"}, moves);

      const std::vector<Square> destinations = {f2, g3};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Nw, h1, to))), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectKnightMoves with pieces of same color on destination squares";

      std::vector<Move> moves;
      collectKnightMoves(Nw, d4, Position{"Nwd4 wc2 Rwb5"}, moves);

      const std::vector<Square> destinations = {c6, e6, e2, b3, f3, f5};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Nw, d4, to))), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectKnightMoves with pieces of opposite color on destination squares";

      std::vector<Move> moves;
      collectKnightMoves(Nw, d4, Position{"Nwd4 bc2 Rbb5"}, moves);

      const std::vector<Square> destinations = {c6, e6, e2, b3, f3, f5};
      std::vector<Move> expected;
      std::transform(std::begin(destinations), std::end(destinations),
                     std::back_inserter(expected),
                     [](Square to) { return BasicMove(Relocation(Nw, d4, to)); });
      expected.push_back(BasicMove(Relocation(Nw, d4, c2), Pb));
      expected.push_back(BasicMove(Relocation(Nw, d4, b5), Rb));

      VERIFY(moves.size() == expected.size(), caseLabel);
      for (const auto& m : expected)
         VERIFY(contains(moves, m), caseLabel);
   }
   {
      const std::string caseLabel = "collectKnightMoves - jumping over pieces";

      std::vector<Move> moves;
      collectKnightMoves(Nw, h1, Position{"Nwh1 wg1 Rwg2 bh2"}, moves);

      const std::vector<Square> destinations = {f2, g3};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Nw, h1, to))), caseLabel);
   }
}


void testCollectPawnMoves()
{
   {
      const std::string caseLabel = "collectPawnMoves for moving forward by one square";

      std::vector<Move> moves;
      collectPawnMoves(Pw, d4, Position{"wd4"}, moves);

      VERIFY(contains(moves, BasicMove(Relocation(Pw, d4, d5))), caseLabel);
   }
   {
      const std::string caseLabel = "collectPawnMoves for moving forward by two squares";

      std::vector<Move> moves;
      collectPawnMoves(Pb, g7, Position{"bg7"}, moves);

      VERIFY(contains(moves, BasicMove(Relocation(Pb, g7, g6))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Pb, g7, g5), EnablesEnPassant)),
             caseLabel);
   }
   {
      const std::string caseLabel =
         "collectPawnMoves with piece of same color in front of pawn";

      std::vector<Move> moves;
      collectPawnMoves(Pw, d4, Position{"wd4 wd5"}, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectPawnMoves with piece of opposite color in front of pawn";

      std::vector<Move> moves;
      collectPawnMoves(Pw, d4, Position{"wd4 bd5"}, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel = "collectPawnMoves with piece of same color two "
                                    "squares in front of pawn on initial rank";

      std::vector<Move> moves;
      collectPawnMoves(Pw, d2, Position{"wd2 wd4"}, moves);

      // Should not move by two squares.
      const std::vector<Square> destinations = {d3};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Pw, d2, to))), caseLabel);
   }
   {
      const std::string caseLabel = "collectPawnMoves with piece of opposite color two "
                                    "squares in front of pawn on initial rank";

      std::vector<Move> moves;
      collectPawnMoves(Pw, d2, Position{"wd2 bd4"}, moves);

      // Should not move by two squares.
      const std::vector<Square> destinations = {d3};
      VERIFY(moves.size() == destinations.size(), caseLabel);
      for (auto to : destinations)
         VERIFY(contains(moves, BasicMove(Relocation(Pw, d2, to))), caseLabel);
   }
   {
      const std::string caseLabel = "collectPawnMoves for taking dialgonal on lower file";

      std::vector<Move> moves;
      collectPawnMoves(Pb, d5, Position{"bd5 Rwc4"}, moves);

      VERIFY(contains(moves, BasicMove(Relocation(Pb, d5, d4))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Pb, d5, c4), Rw)), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectPawnMoves for taking dialgonal on higher file";

      std::vector<Move> moves;
      collectPawnMoves(Pw, f3, Position{"wf3 bg4"}, moves);

      VERIFY(contains(moves, BasicMove(Relocation(Pw, f3, f4))), caseLabel);
      VERIFY(contains(moves, BasicMove(Relocation(Pw, f3, g4), Pb)), caseLabel);
   }
   {
      const std::string caseLabel = "collectPawnMoves for forward promotion";

      std::vector<Move> moves;
      collectPawnMoves(Pw, f7, Position{"wf7"}, moves);

      VERIFY(contains(moves, Promotion(Relocation(Pw, f7, f8), Qw)), caseLabel);
      VERIFY(contains(moves, Promotion(Relocation(Pw, f7, f8), Rw)), caseLabel);
      VERIFY(contains(moves, Promotion(Relocation(Pw, f7, f8), Bw)), caseLabel);
      VERIFY(contains(moves, Promotion(Relocation(Pw, f7, f8), Nw)), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectPawnMoves for diagonal promotion to lower file";

      std::vector<Move> moves;
      collectPawnMoves(Pb, b2, Position{"bb2 Bwa1"}, moves);

      VERIFY(contains(moves, Promotion(Relocation(Pb, b2, a1), Qb, Bw)), caseLabel);
      VERIFY(contains(moves, Promotion(Relocation(Pb, b2, a1), Rb, Bw)), caseLabel);
      VERIFY(contains(moves, Promotion(Relocation(Pb, b2, a1), Bb, Bw)), caseLabel);
      VERIFY(contains(moves, Promotion(Relocation(Pb, b2, a1), Nb, Bw)), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectPawnMoves for diagonal promotion to higher file";

      std::vector<Move> moves;
      collectPawnMoves(Pb, b2, Position{"bb2 Bwc1"}, moves);

      VERIFY(contains(moves, Promotion(Relocation(Pb, b2, c1), Qb, Bw)), caseLabel);
      VERIFY(contains(moves, Promotion(Relocation(Pb, b2, c1), Rb, Bw)), caseLabel);
      VERIFY(contains(moves, Promotion(Relocation(Pb, b2, c1), Bb, Bw)), caseLabel);
      VERIFY(contains(moves, Promotion(Relocation(Pb, b2, c1), Nb, Bw)), caseLabel);
   }
}


void testCollectCastlingMoves()
{
   {
      const std::string caseLabel =
         "collectCastlingMoves for white on king-side when castling is possible";

      std::vector<Move> moves;
      collectCastlingMoves(Color::White, Position{"Kwe1 Rwh1"}, moves);

      VERIFY(moves.size() == 1, caseLabel);
      VERIFY(contains(moves, Castling(Kingside, Color::White)), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectCastlingMoves for white on queen-side when castling is possible";

      std::vector<Move> moves;
      collectCastlingMoves(Color::White, Position{"Kwe1 Rwa1"}, moves);

      VERIFY(moves.size() == 1, caseLabel);
      VERIFY(contains(moves, Castling(Queenside, Color::White)), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectCastlingMoves for black on king-side when castling is possible";

      std::vector<Move> moves;
      collectCastlingMoves(Color::Black, Position{"Kbe8 Rbh8"}, moves);

      VERIFY(moves.size() == 1, caseLabel);
      VERIFY(contains(moves, Castling(Kingside, Color::Black)), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectCastlingMoves for black on queen-side when castling is possible";

      std::vector<Move> moves;
      collectCastlingMoves(Color::Black, Position{"Kbe8 Rba8"}, moves);

      VERIFY(moves.size() == 1, caseLabel);
      VERIFY(contains(moves, Castling(Queenside, Color::Black)), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves when both ways are possible";

      std::vector<Move> moves;
      collectCastlingMoves(Color::Black, Position{"Kbe8 Rba8 Rbh8"}, moves);

      VERIFY(moves.size() == 2, caseLabel);
      VERIFY(contains(moves, Castling(Kingside, Color::Black)), caseLabel);
      VERIFY(contains(moves, Castling(Queenside, Color::Black)), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves when there is no rook";

      std::vector<Move> moves;
      collectCastlingMoves(Color::White, Position{"Kwe1"}, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectCastlingMoves when there is another piece instead of the rook";

      std::vector<Move> moves;
      collectCastlingMoves(Color::White, Position{"Kwe1 Bwa1"}, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves when rook is of wrong color";

      std::vector<Move> moves;
      collectCastlingMoves(Color::Black, Position{"Kbe8 Rwh8"}, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves when rook has moved";

      Position pos{"Kbe8 Rbh8"};
      // Move rook away and back.
      Move rookAway{BasicMove{Relocation{"Rbh8h7"}}};
      makeMove(pos, rookAway);
      Move rookBack{BasicMove{Relocation{"Rbh7h8"}}};
      makeMove(pos, rookBack);

      std::vector<Move> moves;
      collectCastlingMoves(Color::Black, pos, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectCastlingMoves when king is not on initial square";

      std::vector<Move> moves;
      collectCastlingMoves(Color::Black, Position{"Kbe7 Rbh8"}, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectCastlingMoves when king is on initial square but has moved earlier";

      Position pos{"Kwe1 Rwa1"};
      // Move king away and back.
      Move kingAway{BasicMove{Relocation{"Kwe1e2"}}};
      makeMove(pos, kingAway);
      Move kingBack{BasicMove{Relocation{"Kwe2e1"}}};
      makeMove(pos, kingBack);

      std::vector<Move> moves;
      collectCastlingMoves(Color::White, pos, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectCastlingMoves for white when intermediate square is occupied";

      std::vector<Move> moves;
      collectCastlingMoves(Color::White, Position{"Kwe1 Bwc1 Rwa1"}, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectCastlingMoves for black when intermediate square is occupied";

      std::vector<Move> moves;
      collectCastlingMoves(Color::Black, Position{"Kbe8 Qwg8 Rbh8"}, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves for white queen-side when "
                                    "intermediate square d1 is attacked";

      std::vector<Move> moves;
      collectCastlingMoves(Color::White, Position{"Kwe1 Rbd8 Rwa1"}, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves for white queen-side when "
                                    "intermediate square c1 is attacked";

      std::vector<Move> moves;
      collectCastlingMoves(Color::White, Position{"Kwe1 Qbc4 Rwa1"}, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves for white queen-side when "
                                    "intermediate square b1 is attacked";

      std::vector<Move> moves;
      collectCastlingMoves(Color::White, Position{"Kwe1 Rbb8 Rwa1"}, moves);

      // The b1 square being attacked does not prevent castling because the king does
      // not move across that square.
      VERIFY(moves.size() == 1, caseLabel);
      VERIFY(contains(moves, Castling(Queenside, Color::White)), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves for white queen-side when "
                                    "rook square a1 is attacked";

      std::vector<Move> moves;
      collectCastlingMoves(Color::White, Position{"Kwe1 Rba8 Rwa1"}, moves);

      // The rook being attacked does not prevent castling.
      VERIFY(moves.size() == 1, caseLabel);
      VERIFY(contains(moves, Castling(Queenside, Color::White)), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves for white king-side when "
                                    "intermediate square f1 is attacked";

      std::vector<Move> moves;
      collectCastlingMoves(Color::White, Position{"Kwe1 Rbf8 Rwh1"}, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves for white king-side when "
                                    "intermediate square g1 is attacked";

      std::vector<Move> moves;
      collectCastlingMoves(Color::White, Position{"Kwe1 Rbg4 Rwh1"}, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves for white king-side when "
                                    "rook square h1 is attacked";

      std::vector<Move> moves;
      collectCastlingMoves(Color::White, Position{"Kwe1 Rbh8 Rwh1"}, moves);

      // The rook being attacked does not prevent castling.
      VERIFY(moves.size() == 1, caseLabel);
      VERIFY(contains(moves, Castling(Kingside, Color::White)), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves for black queen-side when "
                                    "intermediate square d8 is attacked";

      std::vector<Move> moves;
      collectCastlingMoves(Color::Black, Position{"Kbe8 Rwd1 Rba8"}, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves for black queen-side when "
                                    "intermediate square c8 is attacked";

      std::vector<Move> moves;
      collectCastlingMoves(Color::Black, Position{"Kbe8 Qwc2 Rba8"}, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves for black queen-side when "
                                    "intermediate square b8 is attacked";

      std::vector<Move> moves;
      collectCastlingMoves(Color::Black, Position{"Kbe8 Rwb1 Rba8"}, moves);

      // The b8 square being attacked does not prevent castling because the king does
      // not move across that square.
      VERIFY(moves.size() == 1, caseLabel);
      VERIFY(contains(moves, Castling(Queenside, Color::Black)), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves for black queen-side when "
                                    "rook square a8 is attacked";

      std::vector<Move> moves;
      collectCastlingMoves(Color::Black, Position{"Kbe8 Rwa1 Rba8"}, moves);

      // The rook being attacked does not prevent castling.
      VERIFY(moves.size() == 1, caseLabel);
      VERIFY(contains(moves, Castling(Queenside, Color::Black)), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves for black king-side when "
                                    "intermediate square f8 is attacked";

      std::vector<Move> moves;
      collectCastlingMoves(Color::Black, Position{"Kbe8 Rwf1 Rbh8"}, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves for black king-side when "
                                    "intermediate square g8 is attacked";

      std::vector<Move> moves;
      collectCastlingMoves(Color::Black, Position{"Kbe8 Rwg4 Rbh8"}, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel = "collectCastlingMoves for black king-side when "
                                    "rook square h8 is attacked";

      std::vector<Move> moves;
      collectCastlingMoves(Color::Black, Position{"Kbe8 Rwh1 Rbh8"}, moves);

      // The rook being attacked does not prevent castling.
      VERIFY(moves.size() == 1, caseLabel);
      VERIFY(contains(moves, Castling(Kingside, Color::Black)), caseLabel);
   }
}


void testCollectEnPassantMoves()
{
   {
      const std::string caseLabel =
         "collectEnPassantMoves for black when it is not permitted";

      Position pos{"be4 wd3"};
      Move m = BasicMove{Relocation{"wd3d4"}};
      makeMove(pos, m);

      std::vector<Move> moves;
      collectEnPassantMoves(Color::Black, pos, moves);

      VERIFY(moves.empty(), caseLabel);
   }
   {
      const std::string caseLabel = "collectEnPassantMoves for black on lower file";

      Position pos{"be4 wd2"};
      Move m = BasicMove{Relocation{"wd2d4"}, EnablesEnPassant};
      makeMove(pos, m);

      std::vector<Move> moves;
      collectEnPassantMoves(Color::Black, pos, moves);

      VERIFY(moves.size() == 1, caseLabel);
      VERIFY(contains(moves, EnPassant(Relocation(Pb, e4, d3))), caseLabel);
   }
   {
      const std::string caseLabel = "collectEnPassantMoves for black on higher file";

      Position pos{"be4 wf2"};
      Move m = BasicMove{Relocation{"wf2f4"}, EnablesEnPassant};
      makeMove(pos, m);

      std::vector<Move> moves;
      collectEnPassantMoves(Color::Black, pos, moves);

      VERIFY(moves.size() == 1, caseLabel);
      VERIFY(contains(moves, EnPassant(Relocation(Pb, e4, f3))), caseLabel);
   }
   {
      const std::string caseLabel = "collectEnPassantMoves for white on lower file";

      Position pos{"wg5 bf7"};
      Move m = BasicMove{Relocation{"bf7f5"}, EnablesEnPassant};
      makeMove(pos, m);

      std::vector<Move> moves;
      collectEnPassantMoves(Color::White, pos, moves);

      VERIFY(moves.size() == 1, caseLabel);
      VERIFY(contains(moves, EnPassant(Relocation(Pw, g5, f6))), caseLabel);
   }
   {
      const std::string caseLabel = "collectEnPassantMoves for white on higher file";

      Position pos{"wg5 bh7"};
      Move m = BasicMove{Relocation{"bh7h5"}, EnablesEnPassant};
      makeMove(pos, m);

      std::vector<Move> moves;
      collectEnPassantMoves(Color::White, pos, moves);

      VERIFY(moves.size() == 1, caseLabel);
      VERIFY(contains(moves, EnPassant(Relocation(Pw, g5, h6))), caseLabel);
   }
}


void testCollectAttackedByKing()
{
   {
      const std::string caseLabel = "collectAttackedByKing when fully on board";

      std::vector<Square> loc;
      collectAttackedByKing(Kb, b3, Position{"Kbb3"}, loc);

      static constexpr std::array<Square, 8> Expected = {a2, b2, c2, a3, c3, a4, b4, c4};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectAttackedByKing when on vertical side of board";

      std::vector<Square> loc;
      collectAttackedByKing(Kw, a5, Position{"Kwa5"}, loc);

      static constexpr std::array<Square, 5> Expected = {a4, b4, b5, b6, a6};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectAttackedByKing when on horizontal side of board";

      std::vector<Square> loc;
      collectAttackedByKing(Kb, e8, Position{"Kbe8"}, loc);

      static constexpr std::array<Square, 5> Expected = {d8, d7, e7, f7, f8};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel = "collectAttackedByKing when in corner of board";

      std::vector<Square> loc;
      collectAttackedByKing(Kw, h1, Position{"Kwh1"}, loc);

      static constexpr std::array<Square, 3> Expected = {g1, g2, h2};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel = "collectAttackedByKing when occupied by opponent";

      std::vector<Square> loc;
      collectAttackedByKing(Kb, b3, Position{"Kbb3 Bwc2"}, loc);

      static constexpr std::array<Square, 8> Expected = {a2, b2, c2, a3, c3, a4, b4, c4};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel = "collectAttackedByKing when occupied by own piece";

      std::vector<Square> loc;
      collectAttackedByKing(Kb, b3, Position{"Kbb3 Bbc2"}, loc);

      static constexpr std::array<Square, 7> Expected = {a2, b2, a3, c3, a4, b4, c4};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
}


void testCollectAttackedByQueen()
{
   {
      const std::string caseLabel = "collectAttackedByQueen when fully on board";

      std::vector<Square> loc;
      collectAttackedByQueen(Qw, d5, Position{"Qwd5"}, loc);

      static constexpr std::array<Square, 27> Expected = {
         c4, b3, a2, e6, f7, g8, d4, d3, d2, d1, d6, d7, d8, e4,
         f3, g2, h1, c6, b7, a8, c5, b5, a5, e5, f5, g5, h5};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectAttackedByQueen when on vertical side of board";

      std::vector<Square> loc;
      collectAttackedByQueen(Qb, h6, Position{"Qbh6"}, loc);

      static constexpr std::array<Square, 21> Expected = {g7, f8, a6, b6, c6, d6, e6,
                                                          f6, g6, g5, f4, e3, d2, c1,
                                                          h1, h2, h3, h4, h5, h7, h8};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectAttackedByQueen when on horizontal side of board";

      std::vector<Square> loc;
      collectAttackedByQueen(Qw, d8, Position{"Qwd8"}, loc);

      static constexpr std::array<Square, 21> Expected = {c7, b6, a5, a8, b8, c8, e8,
                                                          f8, g8, h8, e7, f6, g5, h4,
                                                          d7, d6, d5, d4, d3, d2, d1};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel = "collectAttackedByQueen when in corner of board";

      std::vector<Square> loc;
      collectAttackedByQueen(Qb, a1, Position{"Qba1"}, loc);

      static constexpr std::array<Square, 21> Expected = {a2, a3, a4, a5, a6, a7, a8,
                                                          b2, c3, d4, e5, f6, g7, h8,
                                                          b1, c1, d1, e1, f1, g1, h1};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel = "collectAttackedByQueen when blocked by opponent";

      std::vector<Square> loc;
      collectAttackedByQueen(Qw, b3, Position{"Qwb3 Nbd3"}, loc);

      static constexpr std::array<Square, 19> Expected = {
         a2, c4, d5, e6, f7, g8, b1, b2, b4, b5, b6, b7, b8, a4, c2, d1, c3, d3, a3};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel = "collectAttackedByQueen when blocked by own piece";

      std::vector<Square> loc;
      collectAttackedByQueen(Qb, b3, Position{"Qbb3 Nbd3"}, loc);

      static constexpr std::array<Square, 18> Expected = {
         a2, c4, d5, e6, f7, g8, b1, b2, b4, b5, b6, b7, b8, a4, c2, d1, c3, a3};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
}


void testCollectAttackedByRook()
{
   {
      const std::string caseLabel = "collectAttackedByRook when fully on board";

      std::vector<Square> loc;
      collectAttackedByRook(Rw, d5, Position{"Rwd5"}, loc);

      static constexpr std::array<Square, 14> Expected = {d4, d3, d2, d1, d6, d7, d8,
                                                          c5, b5, a5, e5, f5, g5, h5};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectAttackedByRook when on vertical side of board";

      std::vector<Square> loc;
      collectAttackedByRook(Rb, h6, Position{"Rbh6"}, loc);

      static constexpr std::array<Square, 14> Expected = {a6, b6, c6, d6, e6, f6, g6,
                                                          h1, h2, h3, h4, h5, h7, h8};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectAttackedByRook when on horizontal side of board";

      std::vector<Square> loc;
      collectAttackedByRook(Rw, d8, Position{"Rwd8"}, loc);

      static constexpr std::array<Square, 14> Expected = {a8, b8, c8, e8, f8, g8, h8,
                                                          d7, d6, d5, d4, d3, d2, d1};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel = "collectAttackedByRook when in corner of board";

      std::vector<Square> loc;
      collectAttackedByRook(Rb, a1, Position{"Rba1"}, loc);

      static constexpr std::array<Square, 14> Expected = {a2, a3, a4, a5, a6, a7, a8,
                                                          b1, c1, d1, e1, f1, g1, h1};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel = "collectAttackedByRook when blocked by opponent";

      std::vector<Square> loc;
      collectAttackedByRook(Rw, b3, Position{"Rwb3 Nbd3"}, loc);

      static constexpr std::array<Square, 10> Expected = {b1, b2, b4, b5, b6,
                                                          b7, b8, c3, d3, a3};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel = "collectAttackedByRook when blocked by own piece";

      std::vector<Square> loc;
      collectAttackedByRook(Rb, b3, Position{"Rbb3 Nbd3"}, loc);

      static constexpr std::array<Square, 9> Expected = {b1, b2, b4, b5, b6,
                                                         b7, b8, c3, a3};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
}


void testCollectAttackedByBishop()
{
   {
      const std::string caseLabel = "collectAttackedByBishop when fully on board";

      std::vector<Square> loc;
      collectAttackedByBishop(Bw, d5, Position{"Bwd5"}, loc);

      static constexpr std::array<Square, 13> Expected = {c4, b3, a2, e6, f7, g8, e4,
                                                          f3, g2, h1, c6, b7, a8};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectAttackedByBishop when on vertical side of board";

      std::vector<Square> loc;
      collectAttackedByBishop(Bb, h6, Position{"Bbh6"}, loc);

      static constexpr std::array<Square, 7> Expected = {g7, f8, g5, f4, e3, d2, c1};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel =
         "collectAttackedByBishop when on horizontal side of board";

      std::vector<Square> loc;
      collectAttackedByBishop(Bw, d8, Position{"Bwd8"}, loc);

      static constexpr std::array<Square, 7> Expected = {c7, b6, a5, e7, f6, g5, h4};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel = "collectAttackedByBishop when in corner of board";

      std::vector<Square> loc;
      collectAttackedByBishop(Bb, a1, Position{"Bba1"}, loc);

      static constexpr std::array<Square, 7> Expected = {b2, c3, d4, e5, f6, g7, h8};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel = "collectAttackedByBishop when blocked by opponent";

      std::vector<Square> loc;
      collectAttackedByBishop(Bw, b3, Position{"Bwb3 Nbd5"}, loc);

      static constexpr std::array<Square, 6> Expected = {a2, c4, d5, a4, c2, d1};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
   {
      const std::string caseLabel = "collectAttackedByBishop when blocked by own piece";

      std::vector<Square> loc;
      collectAttackedByBishop(Bb, b3, Position{"Bbb3 Nbd5"}, loc);

      static constexpr std::array<Square, 5> Expected = {a2, c4, a4, c2, d1};

      VERIFY(loc.size() == Expected.size(), caseLabel);
      for (Square sq : Expected)
         VERIFY(contains(loc, sq), caseLabel);
   }
}

} // namespace


///////////////////

void testRules()
{
   testCollectKingMoves();
   testCollectQueenMoves();
   testCollectRookMoves();
   testCollectBishopMoves();
   testCollectKnightMoves();
   testCollectPawnMoves();
   testCollectCastlingMoves();
   testCollectEnPassantMoves();
   testCollectAttackedByKing();
   testCollectAttackedByQueen();
   testCollectAttackedByRook();
   testCollectAttackedByBishop();
}
