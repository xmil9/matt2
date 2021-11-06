//
// Aug-2021, Michael Lindner
// MIT license
//
#include "game_tests.h"
#include "game.h"
#include "test_util.h"
#include <stdexcept>

using namespace matt2;


namespace
{
///////////////////

void testDefaultCtor()
{
   {
      const std::string caseLabel = "Game::Game()";

      Game g;
      VERIFY(g.countMoves() == 0, caseLabel);
      VERIFY(g.currentMoveIdx() == 0, caseLabel);
   }
}


void testPositionCtor()
{
   {
      const std::string caseLabel = "Game::Game(Position)";

      Game g{Position{"Kba1 Kwh8"}};
      VERIFY(g.countMoves() == 0, caseLabel);
      VERIFY(g.currentMoveIdx() == 0, caseLabel);
   }
}


void testCalcNextMove()
{
   {
      const std::string caseLabel =
         "Game::calcNextMove in 1 turn for black when taking the highest piece is best";

      Game g{Position{"Kwb1 Rwe4 Bwg4 Kbd8 Bbf5"}};
      const auto& nextPos = g.calcNextMove(Color::Black, 1);

      VERIFY(nextPos == Position{"Kwb1 Bwg4 Kbd8 Bbe4"}, caseLabel);
      VERIFY(g.countMoves() == 1, caseLabel);
      VERIFY(g.getMove(0) == Move(BasicMove{Relocation{Bb, f5, e4}, Rw}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Game::calcNextMove in 1 turn for white with checkmate";

      Game g{Position{"Kwa1 Kbd4 wc3"}};
      const auto& nextPos = g.calcNextMove(Color::White, 1);

      VERIFY(nextPos == Position{"Kwa1 wd4"}, caseLabel);
      VERIFY(g.countMoves() == 1, caseLabel);
      VERIFY(g.getMove(0) == Move(BasicMove{Relocation{Pw, c3, d4}, Kb}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Game::calcNextMove in 1 turn for black with best move not the "
         "one that takes the highest piece";

      // If black took the knight the black queen would be taken by the opponent.
      Game g{Position{"Kwb2 Bwg3 Nwf4 wb6 Kbe8 Qbd6"}};
      const auto& nextPos = g.calcNextMove(Color::Black, 1);

      VERIFY(nextPos == Position{"Kwb2 Bwg3 Nwf4 Kbe8 Qbb6"}, caseLabel);
      VERIFY(g.countMoves() == 1, caseLabel);
      VERIFY(g.getMove(0) == Move(BasicMove{Relocation{Qb, d6, b6}, Pw}), caseLabel);
   }
}

} // namespace


///////////////////

void testGame()
{
   testDefaultCtor();
   testPositionCtor();
   testCalcNextMove();
}
