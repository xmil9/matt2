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

} // namespace


///////////////////

void testRules()
{
   testCollectKingMoves();
   testCollectQueenMoves();
   testCollectRookMoves();
}
