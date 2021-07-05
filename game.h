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
   const Position& calcNextMove(Color side);

 private:
   void apply(Move& m);

 private:
   // Current position.
   Position m_currPos;
   // History of moves.
   std::vector<Move> m_moves;
   // Index of move that leads to current position.
   std::size_t m_currMove = 0;
};


inline const Position& Game::forward()
{
   makeMove(m_currPos, m_moves[++m_currMove]);
   return m_currPos;
}

inline const Position& Game::backward()
{
   reverseMove(m_currPos, m_moves[m_currMove--]);
   return m_currPos;
}

} // namespace matt2
