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
   
} // namespace


///////////////////

void testGame()
{
   testDefaultCtor();
   testPositionCtor();
}
