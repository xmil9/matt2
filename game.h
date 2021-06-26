//
// May-2021, Michael Lindner
// MIT license
//
#pragma once
#include "move.h"
#include "position.h"
#include <cstddef>
#include <vector>


namespace matt2
{
///////////////////

class Game
{
 public:
   std::size_t countMoves() const { return m_moves.size(); }
   std::size_t currentMoveIdx() const { return m_currMove; }

   const Position& forward();
   const Position& backward();

 private:
   // Current position.
   Position m_currPos;
   // History of moves.
   std::vector<Move> m_moves;
   // Index of move that leads to current position.
   std::size_t m_currMove = 0;
};

} // namespace matt2
