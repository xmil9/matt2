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
   return c == White ? "white" : "black";
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

void printCalculatingStatus(Color side, size_t plyDepth, const Position& pos)
{
#ifdef ENABLE_PRINTING
   std::string s = "Calculating move for ";
   s += ::toString(side);
   s += " with depth ";
   s += std::to_string(plyDepth);
   s += " at position ";
   printPosition(s, pos);
   consoleOut(s);
#else
   side;
   plyDepth;
   pos;
#endif
}

void printCalculatedStatus(Color side, size_t plyDepth, const std::optional<Move>& move,
                           double score)
{
#ifdef ENABLE_PRINTING
   std::string s = "Calculated move for ";
   s += ::toString(side);
   s += " with depth ";
   s += std::to_string(plyDepth);
   s += " ==> ";
   s += toString(move, score);
   consoleOut(s);
#else
   side;
   plyDepth;
   move;
   score;
#endif
}

void printEvaluatingStatus(Color side, size_t plyDepth, size_t moveIdx_0based,
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
   s += std::to_string(plyDepth);
   s += ": ";
   s += ::toString(move);
   printPosition(s, pos);
   consoleOut(s);
#else
   side;
   plyDepth;
   moveIdx_0based;
   numMoves;
   move;
   pos;
#endif
}

void printEvaluatedStatus(Color side, size_t plyDepth, size_t moveIdx_0based,
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
   s += std::to_string(plyDepth);
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
   plyDepth;
   moveIdx_0based;
   numMoves;
   move;
   score;
   isBetterMove;
#endif
}

void printPruningStatus(Color side, size_t plyDepth, size_t moveIdx_0based,
                        size_t numMoves, const Move& move, double score,
                        double bestOpposingScore)
{
#ifdef ENABLE_PRINTING
   std::string s = "Pruning after move #";
   s += std::to_string(moveIdx_0based + 1);
   s += "/";
   s += std::to_string(numMoves);
   s += " for ";
   s += ::toString(side);
   s += " with depth ";
   s += std::to_string(plyDepth);
   s += ": ";
   s += ::toString(move);
   s += " ==> score=";
   s += std::to_string(score);
   s += ", best known opponent score=";
   s += std::to_string(bestOpposingScore);
   consoleOut(s);
#else
   side;
   plyDepth;
   moveIdx_0based;
   numMoves;
   move;
   score;
   bestOpposingScore;
#endif
}

///////////////////

// Calculates the next move for a given position.
class MoveCalculator
{
 public:
   MoveCalculator(Position& pos);

   std::optional<Move> next(Color side, size_t turns);

 private:
   struct MoveScore
   {
      std::optional<Move> move;
      double score = 0.;

      explicit operator bool() const;
   };

   MoveScore next(Color side, size_t plyDepth, bool calcMax, double bestOpposingScore);
   void collectMoves(Color side, std::vector<Move>& moves) const;

 private:
   Position& m_pos;
};


MoveCalculator::MoveCalculator(Position& pos) : m_pos{pos}
{
}


std::optional<Move> MoveCalculator::next(Color side, size_t turnDepth)
{
   const bool calcMax = side == White;
   return next(side, 2 * turnDepth, calcMax, getWorstScoreValue(!calcMax)).move;
}


MoveCalculator::MoveScore MoveCalculator::next(Color side, size_t plyDepth, bool calcMax,
                                               double bestOpposingScore)
{
   assert(plyDepth > 0);
   if (plyDepth == 0)
      return {};

   printCalculatingStatus(side, plyDepth, m_pos);

   std::vector<Move> moves;
   // Reserve some space to avoid too many allocations.
   moves.reserve(100);
   collectMoves(side, moves);

   MoveScore bestMove{std::nullopt, getWorstScoreValue(calcMax)};

   // Track move index for debugging.
   size_t moveIdx = 0;
   for (auto& m : moves)
   {
      makeMove(m_pos, m);
      printEvaluatingStatus(side, plyDepth, moveIdx, moves.size(), m, m_pos);

      // Find best counter move for opponent if more plies should be explored.
      MoveScore bestCounterMove;
      if (plyDepth > 1)
         bestCounterMove = next(!side, plyDepth - 1, !calcMax, bestMove.score);

      // Score of move becomes the score of the best counter move if one was found,
      // otherwise use the score of the position.
      double moveScore = 0.;
      if (bestCounterMove)
         moveScore = bestCounterMove.score;
      else
         moveScore = m_pos.updateScore();

      // Use current move if it leads to a better score for the player.
      const bool isBetterMove = bt(moveScore, bestMove.score, calcMax);
      if (isBetterMove)
         bestMove = {m, moveScore};

      printEvaluatedStatus(side, plyDepth, moveIdx, moves.size(), m, moveScore,
                           isBetterMove);

      reverseMove(m_pos, m);

      // Alpha-beta pruning.
      // If the passed best opposing score at this point is better-or-equal (for
      // the opponent) than the best score here, then it will always get chosen over
      // whatever score we can find here because any improvements here go in the opposite
      // value direction. Therefore, we can abort checking any further moves here.
      if (cmp(bestOpposingScore, bestMove.score, !calcMax) >= 0)
      {
         printPruningStatus(side, plyDepth, moveIdx, moves.size(), m, moveScore,
                            bestOpposingScore);
         break;
      }

      ++moveIdx;
   }

   printCalculatedStatus(side, plyDepth, bestMove.move, bestMove.score);
   return bestMove;
}


void MoveCalculator::collectMoves(Color side, std::vector<Move>& moves) const
{
   auto endIter = m_pos.end(side);
   for (auto iter = m_pos.begin(side); iter < endIter; ++iter)
      matt2::collectMoves(iter.piece(), iter.at(), m_pos, moves);

   collectCastlingMoves(side, m_pos, moves);
   collectEnPassantMoves(side, m_pos, moves);
}


MoveCalculator::MoveScore::operator bool() const
{
   return move.has_value();
}

} // namespace


namespace matt2
{
///////////////////

const Position& Game::calcNextMove(size_t turnDepth)
{
   MoveCalculator calc{m_currPos};
   auto move = calc.next(m_nextTurn, turnDepth);
   if (move.has_value())
      apply(*move);
   return m_currPos;
}

const Position& Game::enterNextMove(std::string_view movePacnNotation)
{
   auto moveDescr = readMovePacn(movePacnNotation);
   if (!moveDescr)
      return m_currPos;

   auto move = buildMove(*moveDescr);
   if (move.has_value() && isValidMove(*move, m_currPos, m_nextTurn).first)
      apply(*move);

   return m_currPos;
}


std::optional<Position> Game::forward()
{
   if (atEnd())
      return {};

   makeMove(m_currPos, m_moves[++m_currMove]);
   switchTurn();
   return m_currPos;
}

std::optional<Position> Game::backward()
{
   if (atStart())
      return {};

   reverseMove(m_currPos, m_moves[m_currMove--]);
   switchTurn();
   return m_currPos;
}

void Game::trimMoves()
{
   if (!atEnd())
      m_moves.erase(m_moves.begin() + m_currMove + 1, m_moves.end());
}

static std::optional<Move> buildCastlingMoveFromCoords(const MoveDescription& descr,
                                                       Color side)
{
   if (descr.from == Castling::from(side))
   {
      if (descr.to == Castling::to(Kingside, side))
         return Castling{Kingside, side};
      if (descr.to == Castling::to(Queenside, side))
         return Castling{Queenside, side};
   }

   return {};
}

static std::optional<Move> buildCastlingMove(const MoveDescription& descr, Color side)
{
   // If not special castling notation, check for castling coordinates.
   if (!descr.castling)
      return buildCastlingMoveFromCoords(descr, side);

   if (*descr.castling == MoveDescription::Castling::Kingside)
      return Castling{Kingside, side};
   return Castling{Queenside, side};
}

static Piece buildPromotedToPiece(MoveDescription::Promotion promoteTo, Color side)
{
   switch (promoteTo)
   {
   case MoveDescription::Promotion::Queen:
      return side == Color::White ? Qw : Qb;
   case MoveDescription::Promotion::Rook:
      return side == Color::White ? Rw : Rb;
   case MoveDescription::Promotion::Bishop:
      return side == Color::White ? Bw : Bb;
   case MoveDescription::Promotion::Knight:
      return side == Color::White ? Nw : Nb;
   default:
      assert(false && "Unexpected promotion value.");
      return side == Color::White ? Qw : Qb;
   }
}

static std::optional<Move> buildPromotionMove(const MoveDescription& descr,
                                              const Position& pos, Color side)
{
   if (!descr.from || !descr.to || !descr.promoteTo)
      return {};

   const Square from = *descr.from;
   const Square to = *descr.to;

   const auto piece = pos[from];
   if (!piece)
      return {};

   const Piece promoteTo = buildPromotedToPiece(*descr.promoteTo, side);
   const auto taken = pos[to];

   // Validation of fields happens later.
   return Promotion{*piece, from, to, promoteTo, taken};
}

static std::optional<Move> buildEnPassantMove(const MoveDescription& descr,
                                              const Position& pos, Color side)
{
   if (!descr.from || !descr.to)
      return {};

   const Square from = *descr.from;
   const Square to = *descr.to;

   const auto piece = pos[*descr.from];
   if (!piece || !isPawn(*piece) || color(*piece) != side)
      return {};

   // Checks to make sure it's an en-passant move. We don't want to misidentify a move.
   auto enPassantSq = pos.enPassantSquare();
   if (!enPassantSq)
      return {};

   if (!isAdjacent(file(from), file(*enPassantSq)) && file(to) == file(*enPassantSq))
      return {};

   if (rank(from) != rank(*enPassantSq) && rank(to) != nthRank(side, 6))
      return {};

   // Validation of fields happens later.
   return EnPassant{*piece, from, to};
}

static std::optional<Move> buildBasicMove(const MoveDescription& descr,
                                          const Position& pos)
{
   if (!descr.from || !descr.to)
      return {};

   const Square from = *descr.from;
   const Square to = *descr.to;

   const auto piece = pos[*descr.from];
   if (!piece)
      return {};

   const auto taken = pos[to];

   // Validation of fields happens later.
   return BasicMove{*piece, from, to, taken};
}

std::optional<Move> Game::buildMove(const MoveDescription& descr) const
{
   std::optional<Move> move;

   // Castling
   if (move = buildCastlingMove(descr, m_nextTurn); move.has_value())
      return move;

   // Promotion
   if (move = buildPromotionMove(descr, m_currPos, m_nextTurn); move.has_value())
      return move;

   // En-passant
   if (move = buildEnPassantMove(descr, m_currPos, m_nextTurn); move.has_value())
      return move;

   // Basic move
   if (move = buildBasicMove(descr, m_currPos); move.has_value())
      return move;

   return {};
}

void Game::apply(Move& m)
{
   // Update position.
   makeMove(m_currPos, m);

   // Update move history.
   // Discard moves after the current one.
   trimMoves();
   m_moves.push_back(m);
   // Set current move to last one.
   m_currMove = m_moves.size() - 1;

   // Update turn.
   switchTurn();
}

} // namespace matt2
