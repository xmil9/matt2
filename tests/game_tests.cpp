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
      VERIFY(g.currentMoveIdx() == -1, caseLabel);
      VERIFY(g.nextTurn() == White, caseLabel);
   }
}

void testPositionCtor()
{
   {
      const std::string caseLabel = "Game::Game(Position, Color)";

      Game g{Position{"Kba1 Kwh8"}, Black};
      VERIFY(g.countMoves() == 0, caseLabel);
      VERIFY(g.currentMoveIdx() == -1, caseLabel);
      VERIFY(g.nextTurn() == Black, caseLabel);
   }
}

void testNextTurn()
{
   {
      const std::string caseLabel = "Game::nextTurn";

      const Position pos{"Kwb1 Kbd8"};

      VERIFY(Game().nextTurn() == White, caseLabel);
      VERIFY(Game(pos, White).nextTurn() == White, caseLabel);
      VERIFY(Game(pos, Black).nextTurn() == Black, caseLabel);
   }
}

void testCalcNextMove()
{
   int64_t elapsedNsec = 0;
   auto benchmark = new MicroBenchmark{elapsedNsec};

   {
      const std::string caseLabel =
         "Game::calcNextMove in 1 turn for black when taking the highest piece is best";

      Game g{Position{"Kwb1 Rwe4 Bwg4 Kbd8 Bbf5"}, Black};
      const auto& nextPos = g.calcNextMove(1);

      VERIFY(nextPos == Position{"Kwb1 Bwg4 Kbd8 Bbe4"}, caseLabel);
      VERIFY(g.countMoves() == 1, caseLabel);
      VERIFY(g.getMove(0) == Move(BasicMove{Relocation{Bb, f5, e4}, Rw}), caseLabel);
      VERIFY(g.nextTurn() == White, caseLabel);
   }
   {
      const std::string caseLabel =
         "Game::calcNextMove in 1 turn for white with checkmate";

      Game g{Position{"Kwa1 Kbd4 wc3"}, White};
      const auto& nextPos = g.calcNextMove(1);

      VERIFY(nextPos == Position{"Kwa1 wd4"}, caseLabel);
      VERIFY(g.countMoves() == 1, caseLabel);
      VERIFY(g.getMove(0) == Move(BasicMove{Relocation{Pw, c3, d4}, Kb}), caseLabel);
      VERIFY(g.nextTurn() == Black, caseLabel);
   }
   {
      const std::string caseLabel =
         "Game::calcNextMove in 1 turn for black with best move not the "
         "one that takes the highest piece";

      // If black took the knight the black queen would be taken by the opponent.
      Game g{Position{"Kwb2 Bwg3 Nwf4 wb6 Kbe8 Qbd6"}, Black};
      const auto& nextPos = g.calcNextMove(1);

      VERIFY(nextPos == Position{"Kwb2 Bwg3 Nwf4 Kbe8 Qbb6"}, caseLabel);
      VERIFY(g.countMoves() == 1, caseLabel);
      VERIFY(g.getMove(0) == Move(BasicMove{Relocation{Qb, d6, b6}, Pw}), caseLabel);
      VERIFY(g.nextTurn() == White, caseLabel);
   }
   {
      const std::string caseLabel =
         "Game::calcNextMove in 2 turns for mate (note that the mate is in one move but "
         "the algorithm needs to look ahead two turns to score the position correctly)";

      Game g{Position{"Kwc1 Rwf7 Rwg1 Kbc8"}, White};
      const auto& nextPos = g.calcNextMove(2);

      VERIFY(nextPos == Position{"Kwc1 Rwf7 Rwg8 Kbc8"}, caseLabel);
      VERIFY(g.nextTurn() == Black, caseLabel);
   }
   {
      const std::string caseLabel = "Game::calcNextMove for Polgar Chess book problem 1.";

      Game g{Position{"Kwg1 Qwf6 Bwc3 wh2 wg2 wf2 Kbg8 Qbd8 Rbf8 Bbg7 bh7 bg6 bf7"},
             White};
      const auto& nextPos = g.calcNextMove(2);

      VERIFY(nextPos == Position{"Kwg1 Qwg7 Bwc3 wh2 wg2 wf2 Kbg8 Qbd8 Rbf8 bh7 bg6 bf7"},
             caseLabel);
      VERIFY(g.nextTurn() == Black, caseLabel);
   }
   {
      const std::string caseLabel = "Game::calcNextMove for Polgar Chess book problem 2.";

      Game g{Position{"Kwc1 Qwh6 Rwd1 Nwg5 wb2 wc2 we5 Kbg8 Qbc7 Rbc8 Rbf8 bh7 bg6 bf7"},
             White};
      const auto& nextPos = g.calcNextMove(2);

      VERIFY(nextPos ==
                Position{"Kwc1 Qwh7 Rwd1 Nwg5 wb2 wc2 we5 Kbg8 Qbc7 Rbc8 Rbf8 bg6 bf7"},
             caseLabel);
      VERIFY(g.nextTurn() == Black, caseLabel);
   }

   delete benchmark;
   const double elapsedMsec = double(elapsedNsec) / 1000000.;
   std::cout << "Game performance: " << elapsedMsec << " ms.\n";
}

void testEnterNextMove()
{
   {
      const std::string caseLabel = "Game::enterNextMove for valid basic move";

      Game g{Position{"Kwb1 Rwe4 Bwg4 Kbd8 Bbf5"}, Black};
      const auto& nextPos = g.enterNextMove("f5g6");

      VERIFY(nextPos == Position{"Kwb1 Rwe4 Bwg4 Kbd8 Bbg6"}, caseLabel);
      VERIFY(g.nextTurn() == White, caseLabel);
   }
   {
      const std::string caseLabel = "Game::enterNextMove for invalid basic move";

      Position pos{"Kwb1 Rwe4 Bwg4 Kbd8 Bbf5"};
      Game g{pos, Black};
      const auto& nextPos = g.enterNextMove("f4g6");

      // Game state should be the same as before.
      VERIFY(nextPos == pos, caseLabel);
      VERIFY(g.nextTurn() == Black, caseLabel);
   }
   {
      const std::string caseLabel =
         "Game::enterNextMove for valid basic move with taking";

      Game g{Position{"Kwb1 Rwe4 Bwg4 Kbd8 Bbf5 wg6"}, Black};
      const auto& nextPos = g.enterNextMove("f5g6");

      VERIFY(nextPos == Position{"Kwb1 Rwe4 Bwg4 Kbd8 Bbg6"}, caseLabel);
      VERIFY(g.nextTurn() == White, caseLabel);
   }
   {
      const std::string caseLabel =
         "Game::enterNextMove for invalid basic move description";

      const Position initialPos{"Kwb1 Rwe4 Bwg4 Kbd8 Bbf5"};
      Game g{initialPos, Black};
      const auto& nextPos = g.enterNextMove("f56");

      // Nothing changed.
      VERIFY(nextPos == initialPos, caseLabel);
      VERIFY(g.nextTurn() == Black, caseLabel);
   }
   {
      const std::string caseLabel =
         "Game::enterNextMove for invalid basic move with no piece is on 'from' square";

      const Position initialPos{"Kwb1 Rwe4 Bwg4 Kbd8 Bbf5"};
      Game g{initialPos, Black};
      const auto& nextPos = g.enterNextMove("f4g6");

      // Nothing changed.
      VERIFY(nextPos == initialPos, caseLabel);
      VERIFY(g.nextTurn() == Black, caseLabel);
   }
   {
      const std::string caseLabel = "Game::enterNextMove for valid o-o-o castling move";

      Game g{Position{"Kwb1 Kbe8 Rba8"}, Black};
      const auto& nextPos = g.enterNextMove("o-o-o");

      VERIFY(nextPos == Position{"Kwb1 Kbc8 Rbd8"}, caseLabel);
      VERIFY(g.nextTurn() == White, caseLabel);
   }
   {
      const std::string caseLabel = "Game::enterNextMove for valid o-o castling move";

      Game g{Position{"Kwe1 Kbe8 Rwh1"}, White};
      const auto& nextPos = g.enterNextMove("o-o");

      VERIFY(nextPos == Position{"Kwg1 Kbe8 Rwf1"}, caseLabel);
      VERIFY(g.nextTurn() == Black, caseLabel);
   }
   {
      const std::string caseLabel = "Game::enterNextMove for invalid o-o castling move";

      Position pos{"Kwe1 Kbe8 Rwh1"};
      Game g{pos, Black};
      const auto& nextPos = g.enterNextMove("o-o");

      // Game state should be the same as before.
      VERIFY(nextPos == pos, caseLabel);
      VERIFY(g.nextTurn() == Black, caseLabel);
   }
   {
      const std::string caseLabel =
         "Game::enterNextMove for king-side castling move by coordinates";

      Game g{Position{"Kwb1 Kbe8 Rba8"}, Black};
      const auto& nextPos = g.enterNextMove("e8c8");

      VERIFY(nextPos == Position{"Kwb1 Kbc8 Rbd8"}, caseLabel);
      VERIFY(g.nextTurn() == White, caseLabel);
   }
   {
      const std::string caseLabel = "Game::enterNextMove for queen-side castling move by coordinates";

      Game g{Position{"Kwe1 Kbe8 Rwh1"}, White};
      const auto& nextPos = g.enterNextMove("e1g1");

      VERIFY(nextPos == Position{"Kwg1 Kbe8 Rwf1"}, caseLabel);
      VERIFY(g.nextTurn() == Black, caseLabel);
   }
   {
      const std::string caseLabel = "Game::enterNextMove for valid promotion move";

      Game g{Position{"Kwe1 Kbe8 wh7"}, White};
      const auto& nextPos = g.enterNextMove("h7h8q");

      VERIFY(nextPos == Position{"Kwe1 Kbe8 Qwh8"}, caseLabel);
      VERIFY(g.nextTurn() == Black, caseLabel);
   }
   {
      const std::string caseLabel = "Game::enterNextMove for invalid promotion move";

      Position pos{"Kwe1 Kbe8 wh6"};
      Game g{pos, White};
      const auto& nextPos = g.enterNextMove("h7h8q");

      VERIFY(nextPos == pos, caseLabel);
      VERIFY(g.nextTurn() == White, caseLabel);
   }
   {
      const std::string caseLabel =
         "Game::enterNextMove for invalid promotion move description";

      const Position initialPos{"Kwe1 Kbe8 wh7"};
      Game g{initialPos, White};
      const auto& nextPos = g.enterNextMove("g7x8q");

      VERIFY(nextPos == initialPos, caseLabel);
      VERIFY(g.nextTurn() == White, caseLabel);
   }
   {
      const std::string caseLabel =
         "Game::enterNextMove for invalid promotion move with no piece on 'from' square";

      const Position initialPos{"Kwe1 Kbe8 wh7"};
      Game g{initialPos, White};
      const auto& nextPos = g.enterNextMove("g7g8q");

      VERIFY(nextPos == initialPos, caseLabel);
      VERIFY(g.nextTurn() == White, caseLabel);
   }
   {
      const std::string caseLabel = "Game::enterNextMove for valid en-passant move";

      Position initialPos{"Kwe1 Kbe8 wb4 bc4"};
      initialPos.setEnPassantSquare(b4);

      Game g{initialPos, Black};
      const auto& nextPos = g.enterNextMove("c4b3");

      VERIFY(nextPos == Position{"Kwe1 Kbe8 bb3"}, caseLabel);
      VERIFY(g.nextTurn() == White, caseLabel);
   }
   {
      const std::string caseLabel = "Game::enterNextMove for invalid en-passant move";

      Position initialPos{"Kwe1 Kbe8 wb4 bc4"};
      // Position does not have flag to enable en-passant!
      // initialPos.setEnPassantSquare(b4);

      Game g{initialPos, Black};
      const auto& nextPos = g.enterNextMove("c4b3");

      VERIFY(nextPos == initialPos, caseLabel);
      VERIFY(g.nextTurn() == Black, caseLabel);
   }
}

void testCurrent()
{
   {
      const std::string caseLabel = "Game::current";

      const Position pos{"Kwb1 Kbd8"};

      VERIFY(Game().current() == StartPos, caseLabel);
      VERIFY(Game(pos, White).current() == pos, caseLabel);
   }
}

void testForwardAndBackward()
{
   {
      const std::string caseLabel = "Game::forward() and Game::backward()";

      const Position initialPos{"Kwb1 Kbd8"};
      Game g{initialPos, White};
      const Position pos1 = g.enterNextMove("b1b2");
      const Position pos2 = g.enterNextMove("d8d7");

      auto outPos = g.backward();
      VERIFY(outPos.has_value(), caseLabel);
      VERIFY(outPos == pos1, caseLabel);
      VERIFY(g.current() == pos1, caseLabel);
      VERIFY(g.nextTurn() == Black, caseLabel);
      VERIFY(g.currentMoveIdx() == 0, caseLabel);

      outPos = g.backward();
      VERIFY(outPos.has_value(), caseLabel);
      VERIFY(outPos == initialPos, caseLabel);
      VERIFY(g.current() == initialPos, caseLabel);
      VERIFY(g.nextTurn() == White, caseLabel);
      VERIFY(g.currentMoveIdx() == -1, caseLabel);

      // Try backward from initial pos.
      outPos = g.backward();
      VERIFY(!outPos.has_value(), caseLabel);
      VERIFY(g.current() == initialPos, caseLabel);
      VERIFY(g.nextTurn() == White, caseLabel);
      VERIFY(g.currentMoveIdx() == -1, caseLabel);

      outPos = g.forward();
      VERIFY(outPos.has_value(), caseLabel);
      VERIFY(outPos == pos1, caseLabel);
      VERIFY(g.current() == pos1, caseLabel);
      VERIFY(g.nextTurn() == Black, caseLabel);
      VERIFY(g.currentMoveIdx() == 0, caseLabel);

      outPos = g.forward();
      VERIFY(outPos.has_value(), caseLabel);
      VERIFY(outPos == pos2, caseLabel);
      VERIFY(g.current() == pos2, caseLabel);
      VERIFY(g.nextTurn() == White, caseLabel);
      VERIFY(g.currentMoveIdx() == 1, caseLabel);

      // Try forward from last pos.
      outPos = g.forward();
      VERIFY(!outPos.has_value(), caseLabel);
      VERIFY(g.current() == pos2, caseLabel);
      VERIFY(g.nextTurn() == White, caseLabel);
      VERIFY(g.currentMoveIdx() == 1, caseLabel);
   }
}

void testCountMoves()
{
   {
      const std::string caseLabel = "Game::countMoves()";

      const Position initialPos{"Kwb1 Kbd8"};
      Game g{initialPos, White};
      VERIFY(g.countMoves() == 0, caseLabel);

      g.enterNextMove("b1b2");
      VERIFY(g.countMoves() == 1, caseLabel);

      g.enterNextMove("d8d7");
      VERIFY(g.countMoves() == 2, caseLabel);
   }
}

void testGetMove()
{
   {
      const std::string caseLabel = "Game::getMove(size_t)";

      const Position initialPos{"Kwb1 Kbd8"};
      Game g{initialPos, White};
      g.enterNextMove("b1b2");
      g.enterNextMove("d8d7");

      auto move = g.getMove(0);
      VERIFY(from(move) == b1, caseLabel);
      VERIFY(to(move) == b2, caseLabel);

      move = g.getMove(1);
      VERIFY(from(move) == d8, caseLabel);
      VERIFY(to(move) == d7, caseLabel);
   }
}

void testCurrentMoveIdx()
{
   {
      const std::string caseLabel = "Game::currentMoveIdx()";

      const Position initialPos{"Kwb1 Kbd8"};
      Game g{initialPos, White};
      VERIFY(g.currentMoveIdx() == -1, caseLabel);

      g.enterNextMove("b1b2");
      VERIFY(g.currentMoveIdx() == 0, caseLabel);

      g.enterNextMove("d8d7");
      VERIFY(g.currentMoveIdx() == 1, caseLabel);
   }
}

} // namespace


///////////////////

void testGame()
{
   testDefaultCtor();
   testPositionCtor();
   testNextTurn();
   testCalcNextMove();
   testEnterNextMove();
   testCurrent();
   testForwardAndBackward();
   testCountMoves();
   testGetMove();
   testCurrentMoveIdx();
}
