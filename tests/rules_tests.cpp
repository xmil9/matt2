//
// Jul-2021, Michael Lindner
// MIT license
//
#include "rules_tests.h"
#include "rules.h"
#include "test_util.h"
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
}

} // namespace


///////////////////

void testRules()
{
   testCollectKingMoves();
}
