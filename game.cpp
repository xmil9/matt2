//
// Jun-2021, Michael Lindner
// MIT license
//
#include "game.h"
#include "console.h"
#include "notation.h"
#include "rules.h"
#include "scoring.h"
#include <limits>
#include <queue>

using namespace matt2;

// #define ENABLE_PRINTING


namespace
{
///////////////////

std::string toString(Color c)
{
   return c == Color::White ? "white" : "black";
}

std::string toString(const Move& m)
{
   std::string s;
   return notate(s, m, Lan{});
}

std::string toString(const std::optional<Move>& move, double score)
{
   std::string s;
   Lan n;

   if (move)
   {
      notate(s, *move, n);
      s += "(score=" + std::to_string(score) + ")";
   }
   else
   {
      s += "<none>";
   }

   return s;
}

void printCalculatingStatus(Color side, size_t plies, const Position& pos)
{
#ifdef ENABLE_PRINTING
   std::string s = "Calculating move for ";
   s += ::toString(side);
   s += " with depth ";
   s += std::to_string(plies);
   s += " at position ";
   printPosition(s, pos);
   consoleOut(s);
#else
   side;
   plies;
   pos;
#endif
}

void printCalculatedStatus(Color side, size_t plies, const std::optional<Move>& move,
                           double score)
{
#ifdef ENABLE_PRINTING
   std::string s = "Calculated move for ";
   s += ::toString(side);
   s += " with depth ";
   s += std::to_string(plies);
   s += " ==> ";
   s += toString(move, score);
   consoleOut(s);
#else
   side;
   plies;
   move;
   score;
#endif
}

void printEvaluatingStatus(Color side, size_t plies, size_t moveIdx_0based,
                           size_t numMoves, const Move& move, const Position& pos)
{
#ifdef ENABLE_PRINTING
   std::string s = "Evaluating move #";
   s += std::to_string(moveIdx_0based + 1);
   s += "/";
   s += std::to_string(numMoves);
   s += " for ";
   s += ::toString(side);
   s += " with depth ";
   s += std::to_string(plies);
   s += ": ";
   s += ::toString(move);
   printPosition(s, pos);
   consoleOut(s);
#else
   side;
   plies;
   moveIdx_0based;
   numMoves;
   move;
   pos;
#endif
}

void printEvaluatedStatus(Color side, size_t plies, size_t moveIdx_0based,
                          size_t numMoves, const Move& move, double score,
                          bool isBetterMove)
{
#ifdef ENABLE_PRINTING
   std::string s = "Evaluated move #";
   s += std::to_string(moveIdx_0based + 1);
   s += "/";
   s += std::to_string(numMoves);
   s += " for ";
   s += ::toString(side);
   s += " with depth ";
   s += std::to_string(plies);
   s += ": ";
   s += ::toString(move);
   s += " ==> score=";
   s += std::to_string(score);
   if (isBetterMove)
      s += " ==> better move";
   else
      s += " ==> no improvement";
   consoleOut(s);
#else
   side;
   plies;
   moveIdx_0based;
   numMoves;
   move;
   score;
   isBetterMove;
#endif
}

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
      double score = 0.;

      explicit operator bool() const;
   };

   MoveScore next(Color side, std::size_t plies, bool calcMax);
   void collectMoves(Color side, std::vector<Move>& moves) const;
   void collectMoves(Piece piece, Square at, std::vector<Move>& moves) const;

 private:
   Position& m_pos;
};


MoveCalculator::MoveCalculator(Position& pos) : m_pos{pos}
{
}


std::optional<Move> MoveCalculator::next(Color side, std::size_t turns)
{
   return next(side, 2 * turns, side == Color::White).move;
}


MoveCalculator::MoveScore MoveCalculator::next(Color side, std::size_t plies,
                                               bool calcMax)
{
   assert(plies > 0);
   if (plies == 0)
      return {};

   printCalculatingStatus(side, plies, m_pos);

   std::vector<Move> moves;
   // Reserve some space to avoid too many allocations.
   moves.reserve(100);
   collectMoves(side, moves);

   MoveScore bestMove{std::nullopt, getWorstScoreValue(calcMax)};

   // Track move index for debugging.
   std::size_t moveIdx = 0;
   for (auto& m : moves)
   {
      makeMove(m_pos, m);
      printEvaluatingStatus(side, plies, moveIdx, moves.size(), m, m_pos);

      // Calculate score without counter moves.
      double moveScore = m_pos.updateScore();

      // Find best counter move for opponent if more plies should be explored.
      MoveScore bestCounterMove;
      if (plies > 1)
      {
         bestCounterMove = next(!side, plies - 1, !calcMax);
         // Score of move becomes the score of the best counter move if one was found.
         if (bestCounterMove)
            moveScore = bestCounterMove.score;
      }

      // Use move m if it leads to a better score for the player.
      const bool isBetterMove = bt(moveScore, bestMove.score, calcMax);
      if (isBetterMove)
         bestMove = {m, moveScore};

      printEvaluatedStatus(side, plies, moveIdx, moves.size(), m, moveScore,
                           isBetterMove);

      reverseMove(m_pos, m);
      ++moveIdx;
   }

   printCalculatedStatus(side, plies, bestMove.move, bestMove.score);
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

const Position& Game::calcNextMove(Color side, std::size_t turns)
{
   MoveCalculator calc{m_currPos};
   auto move = calc.next(side, turns);
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
