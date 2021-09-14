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

   std::optional<Move> next(Color side, std::size_t turns);

 private:
   struct MoveScore
   {
      std::optional<Move> move;
      double score;

      explicit operator bool() const;
   };

   MoveScore next(Color side, std::size_t plies, bool calcMax);
   void collectMoves(Color side, std::vector<Move>& moves) const;
   void collectMoves(Piece piece, Square at, std::vector<Move>& moves) const;

   static bool isBetterScore(double a, double b, bool calcMax);

 private:
   Position& m_pos;
};


MoveCalculator::MoveCalculator(Position& pos) : m_pos{pos}
{
}


std::optional<Move> MoveCalculator::next(Color side, std::size_t turns)
{
   return next(side, 2 * turns, true).move;
}


bool MoveCalculator::isBetterScore(double a, double b, bool calcMax)
{
   return calcMax ? a > b : a < b;
}


MoveCalculator::MoveScore MoveCalculator::next(Color side, std::size_t plies,
                                               bool calcMax)
{
   if (plies == 0)
      return {std::nullopt, 0.};

   std::vector<Move> moves;
   // Reserve some space to avoid too many allocations.
   moves.reserve(100);
   collectMoves(side, moves);

   MoveScore bestMove{std::nullopt, calcMax ? std::numeric_limits<double>::lowest()
                                            : std::numeric_limits<double>::max()};

   for (auto& m : moves)
   {
      makeMove(m_pos, m);

      // Find best counter move for opponent.
      auto bestCounterMove = next(!side, plies - 1, !calcMax);
      double score = bestCounterMove ? bestCounterMove.score : m_pos.updateScore();
      
      // Use move m if it leads to a better score for the player.
      if (isBetterScore(score, bestMove.score, calcMax))
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


MoveCalculator::MoveScore::operator bool() const
{
   return move.has_value();
}

} // namespace


namespace matt2
{
///////////////////

const Position& Game::calcNextMove(Color side)
{
   MoveCalculator calc{m_currPos};
   auto move = calc.next(side, 1);
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
