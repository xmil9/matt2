//
// Aug-2021, Michael Lindner
// MIT license
//
#include "game_tests.h"
#include "game.h"
#include "micro_benchmark.h"
#include "test_util.h"
#include <iostream>
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
   int64_t elapsedNsec = 0;
   auto benchmark = new MicroBenchmark{elapsedNsec};

   {
      const std::string caseLabel =
         "Game::calcNextMove in 1 turn for black when taking the highest piece is best";

      Game g{Position{"Kwb1 Rwe4 Bwg4 Kbd8 Bbf5"}};
      const auto& nextPos = g.calcNextMove(Black, 1);

      VERIFY(nextPos == Position{"Kwb1 Bwg4 Kbd8 Bbe4"}, caseLabel);
      VERIFY(g.countMoves() == 1, caseLabel);
      VERIFY(g.getMove(0) == Move(BasicMove{Relocation{Bb, f5, e4}, Rw}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Game::calcNextMove in 1 turn for white with checkmate";

      Game g{Position{"Kwa1 Kbd4 wc3"}};
      const auto& nextPos = g.calcNextMove(White, 1);

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
      const auto& nextPos = g.calcNextMove(Black, 1);

      VERIFY(nextPos == Position{"Kwb2 Bwg3 Nwf4 Kbe8 Qbb6"}, caseLabel);
      VERIFY(g.countMoves() == 1, caseLabel);
      VERIFY(g.getMove(0) == Move(BasicMove{Relocation{Qb, d6, b6}, Pw}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Game::calcNextMove in 2 turns for mate (note that the mate is in one move but "
         "the algorithm needs to look ahead two turns to score the position correctly)";

      Game g{Position{"Kwc1 Rwf7 Rwg1 Kbc8"}};
      const auto& nextPos = g.calcNextMove(White, 2);

      VERIFY(nextPos == Position{"Kwc1 Rwf7 Rwg8 Kbc8"}, caseLabel);
   }
   {
      const std::string caseLabel = "Game::calcNextMove for Polgar Chess book problem 1.";

      Game g{Position{"Kwg1 Qwf6 Bwc3 wh2 wg2 wf2 Kbg8 Qbd8 Rbf8 Bbg7 bh7 bg6 bf7"}};
      const auto& nextPos = g.calcNextMove(White, 2);

      VERIFY(nextPos == Position{"Kwg1 Qwg7 Bwc3 wh2 wg2 wf2 Kbg8 Qbd8 Rbf8 bh7 bg6 bf7"},
             caseLabel);
   }
   {
      const std::string caseLabel = "Game::calcNextMove for Polgar Chess book problem 2.";

      Game g{Position{"Kwc1 Qwh6 Rwd1 Nwg5 wb2 wc2 we5 Kbg8 Qbc7 Rbc8 Rbf8 bh7 bg6 bf7"}};
      const auto& nextPos = g.calcNextMove(White, 2);

      VERIFY(nextPos ==
                Position{"Kwc1 Qwh7 Rwd1 Nwg5 wb2 wc2 we5 Kbg8 Qbc7 Rbc8 Rbf8 bg6 bf7"},
             caseLabel);
   }

   delete benchmark;
   const double elapsedMsec = double(elapsedNsec) / 1000000.;
   std::cout << "Game performance: " << elapsedMsec << " ms.\n";
}

} // namespace


///////////////////

void testGame()
{
   testDefaultCtor();
   testPositionCtor();
   testCalcNextMove();
}
