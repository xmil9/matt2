//
// Jun-2021, Michael Lindner
// MIT license
//
#include "game.h"
#include "rules.h"
#include <limits>
#include <queue>

using namespace matt2;


namespace
{
///////////////////

class MoveCalculator
{
 public:
   MoveCalculator(Position& pos);

   std::optional<Move> next(Color side);

 private:
   struct MoveScore
   {
      std::optional<Move> move;
      double score;
   };

   MoveScore next(Color side, bool calcMax);
   void collectMoves(Color side, std::vector<Move>& moves) const;
   void collectMoves(Piece piece, Square at, std::vector<Move>& moves) const;

   static bool isBetterScore(double a, double b, bool calcMax);

 private:
   Position& m_pos;
};


MoveCalculator::MoveCalculator(Position& pos) : m_pos{pos}
{
}


std::optional<Move> MoveCalculator::next(Color side)
{
   return next(side, true).move;
}


bool MoveCalculator::isBetterScore(double a, double b, bool calcMax)
{
   return calcMax ? a > b : a < b;
}


MoveCalculator::MoveScore MoveCalculator::next(Color side, bool calcMax)
{
   std::vector<Move> moves;
   // Reserve some space to avoid too many allocations.
   moves.reserve(100);
   collectMoves(side, moves);

   MoveScore bestMove{std::nullopt, calcMax ? 0. : std::numeric_limits<double>::max()};

   for (auto& m : moves)
   {
      makeMove(m_pos, m);

      auto bestCounterMove = next(!side, !calcMax);
      if (isBetterScore(bestCounterMove.score, bestMove.score, calcMax))
         bestMove = {m, bestCounterMove.score};

      reverseMove(m_pos, m);
   }

   return bestMove;
}


void MoveCalculator::collectMoves(Color side, std::vector<Move>& moves) const
{
   auto endIter = m_pos.end(side);
   for (auto iter = m_pos.begin(side); iter < endIter; ++iter)
      collectMoves(iter.piece(), iter.at(), moves);

   collectCastlingMoves(side, m_pos, moves);
   collectEnPassantMoves(side, m_pos, moves);
}


void MoveCalculator::collectMoves(Piece piece, Square at, std::vector<Move>& moves) const
{
   if (isKing(piece))
      collectKingMoves(piece, at, m_pos, moves);
   else if (isQueen(piece))
      collectQueenMoves(piece, at, m_pos, moves);
   else if (isRook(piece))
      collectRookMoves(piece, at, m_pos, moves);
   else if (isBishop(piece))
      collectBishopMoves(piece, at, m_pos, moves);
   else if (isKnight(piece))
      collectKnightMoves(piece, at, m_pos, moves);
   else if (isPawn(piece))
      collectPawnMoves(piece, at, m_pos, moves);
   else
      throw std::runtime_error("Unknown piece.");
}

} // namespace


namespace matt2
{
///////////////////

const Position& Game::calcNextMove(Color side)
{
   MoveCalculator calc{m_currPos};
   auto move = calc.next(side);
   if (move.has_value())
      apply(*move);
   return m_currPos;
}

void Game::apply(Move& m)
{
   makeMove(m_currPos, m);
   m_moves.push_back(m);
   m_currMove = m_moves.size() - 1;
}

} // namespace matt2
