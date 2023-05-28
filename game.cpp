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
#include <variant>

using namespace matt2;

//#define ENABLE_PRINTING


namespace
{
///////////////////

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

#ifdef ENABLE_PRINTING
void printCalculatingStatus(Color side, size_t plyDepth, const Position& pos)
{
   std::string s = "Calculating move for ";
   s += toString(side);
   s += " with depth ";
   s += std::to_string(plyDepth);
   s += " at position ";
   printPosition(s, pos);
   consoleOut(s);
}
#else
void printCalculatingStatus(Color /*side*/, size_t /*plyDepth*/, const Position& /*pos*/)
{
}
#endif // ENABLE_PRINTING

#ifdef ENABLE_PRINTING
void printCalculatedStatus(Color side, size_t plyDepth, const std::optional<Move>& move,
                           double score)
{
   std::string s = "Calculated move for ";
   s += ::toString(side);
   s += " with depth ";
   s += std::to_string(plyDepth);
   s += " ==> ";
   s += toString(move, score);
   consoleOut(s);
}
#else
void printCalculatedStatus(Color /*side*/, size_t /*plyDepth*/,
                           const std::optional<Move>& /*move*/, double /*score*/)
{
}
#endif // ENABLE_PRINTING

#ifdef ENABLE_PRINTING
void printEvaluatingStatus(Color side, size_t plyDepth, size_t moveIdx_0based,
                           size_t numMoves, const Move& move, const Position& pos)
{
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
}
#else
void printEvaluatingStatus(Color /*side*/, size_t /*plyDepth*/, size_t /*moveIdx_0based*/,
                           size_t /*numMoves*/, const Move& /*move*/,
                           const Position& /*pos*/)
{
}
#endif // ENABLE_PRINTING

#ifdef ENABLE_PRINTING
void printEvaluatedStatus(Color side, size_t plyDepth, size_t moveIdx_0based,
                          size_t numMoves, const Move& move, double score,
                          bool isBetterMove)
{
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
}
#else
void printEvaluatedStatus(Color /*side*/, size_t /*plyDepth*/, size_t /*moveIdx_0based*/,
                          size_t /*numMoves*/, const Move& /*move*/, double /*score*/,
                          bool /*isBetterMove*/)
{
}
#endif // ENABLE_PRINTING

#ifdef ENABLE_PRINTING
void printPruningStatus(Color side, size_t plyDepth, size_t moveIdx_0based,
                        size_t numMoves, const Move& move, double score,
                        double bestOpposingScore)
{
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
}
#else
void printPruningStatus(Color /*side*/, size_t /*plyDepth*/, size_t /*moveIdx_0based*/,
                        size_t /*numMoves*/, const Move& /*move*/, double /*score*/,
                        double /*bestOpposingScore*/)
{
}
#endif // ENABLE_PRINTING

///////////////////

// Calculates the next move for a given position.
class MoveCalculator
{
 public:
   MoveCalculator(Position& pos);

   std::optional<Move> next(Color side, size_t plyDepth);

 private:
   struct MoveScore
   {
      std::optional<Move> move;
      double score = 0.;
   };
   struct MaxDepthReached
   {
   };
   struct NoValidMoveFound
   {
   };
   using MoveResult = std::variant<MoveScore, MaxDepthReached, NoValidMoveFound>;

   MoveResult next(Color side, size_t plyDepth, bool calcMax, double bestOpposingScore);
   void collectMoves(Color side, std::vector<Move>& moves) const;

 private:
   Position& m_pos;
   size_t m_totalPlies = 0;
};


MoveCalculator::MoveCalculator(Position& pos) : m_pos{pos}
{
}


std::optional<Move> MoveCalculator::next(Color side, size_t plyDepth)
{
   m_totalPlies = plyDepth;
   const bool calcMax = side == White;
   const MoveResult result = next(side, plyDepth, calcMax, getWorstScoreValue(!calcMax));
   if (std::holds_alternative<MoveScore>(result))
      return std::get<MoveScore>(result).move;
   return {};
}


MoveCalculator::MoveResult MoveCalculator::next(Color side, size_t plyDepth, bool calcMax,
                                                double bestOpposingScore)
{
   assert(plyDepth > 0);
   if (plyDepth == 0)
      return MaxDepthReached{};

   printCalculatingStatus(side, plyDepth, m_pos);

   // Collect all possible moves.
   std::vector<Move> moves;
   // Reserve some space to avoid too many allocations.
   moves.reserve(100);
   collectMoves(side, moves);
   if (moves.empty())
      return NoValidMoveFound{};

   // Find best move.
   MoveScore bestMove{std::nullopt, getWorstScoreValue(calcMax)};

   // Track move index for debugging.
   size_t moveIdx = 0;

   for (auto& m : moves)
   {
      makeMove(m_pos, m);
      printEvaluatingStatus(side, plyDepth, moveIdx, moves.size(), m, m_pos);

      // Find best counter move for opponent, if more plies should be explored.
      MoveResult bestCounterMove = MaxDepthReached{};
      if (plyDepth > 1)
         bestCounterMove = next(!side, plyDepth - 1, !calcMax, bestMove.score);

      // Score of move becomes the score of the best counter move if one was found.
      double moveScore = 0.;
      if (std::holds_alternative<MoveScore>(bestCounterMove))
      {
         moveScore = std::get<MoveScore>(bestCounterMove).score;
      }
      // If there is no counter move because the max depth has been reached,
      // use the score of the position as score of the current move.
      else if (std::holds_alternative<MaxDepthReached>(bestCounterMove))
      {
         moveScore = m_pos.updateScore();
      }
      // If there is no counter move because no legal move is possible,
      // it's either a mate or a tie.
      else if (std::holds_alternative<NoValidMoveFound>(bestCounterMove))
      {
         if (isCheck(!side, m_pos))
            moveScore = calcMateScore(!side, m_pos, m_totalPlies - plyDepth);
         else
            moveScore = calcTieScore(side, m_pos);
      }
      else
      {
         throw std::runtime_error("Unexpected move result.");
      }

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

static void removeIfCheck(std::vector<Move>& moves, Position pos, Color side)
{
   std::erase_if(moves,
                 [&pos, side](Move& m)
                 {
                    makeMove(pos, m);
                    const bool leadsToCheck = isCheck(side, pos);
                    reverseMove(pos, m);
                    return leadsToCheck;
                 });
}

void MoveCalculator::collectMoves(Color side, std::vector<Move>& moves) const
{
   const auto endIter = m_pos.end(side);
   for (auto iter = m_pos.begin(side); iter < endIter; ++iter)
      matt2::collectMoves(iter.piece(), iter.at(), m_pos, moves);

   collectCastlingMoves(side, m_pos, moves);
   collectEnPassantMoves(side, m_pos, moves);

   // Eliminate moves that would lead to check.
   removeIfCheck(moves, m_pos, side);
}

///////////////////

std::string describeMove(const Move& move)
{
   std::string notation;
   return Lan{}.notate(notation, move);
}

} // namespace


namespace matt2
{
///////////////////

std::pair<bool, std::string> Game::calcNextMove(size_t turnDepth)
{
   if (isMate(m_nextTurn))
      return {false, "Cannot move when mate."};

   MoveCalculator calc{m_currPos};
   auto move = calc.next(m_nextTurn, 2 * turnDepth);
   if (!move)
      return {false, "No move found."};

   apply(*move);
   return {true, describeMove(*move)};
}

std::pair<bool, std::string> Game::enterNextMove(std::string_view movePacnNotation)
{
   if (isMate(m_nextTurn))
      return {false, "Cannot move when mate."};

   // Parse move notation to internal move description.
   auto moveDescr = readMovePacn(movePacnNotation);
   if (!moveDescr)
      return {false, "Invalid move notation."};

   // Build a move from the description.
   auto [move, errText] = buildMove(*moveDescr);
   if (!move)
      return {false, errText};

   // Check validity in context of game.
   bool isValid = false;
   std::tie(isValid, errText) = isValidMove(*move, m_currPos, m_nextTurn);
   if (!isValid)
      return {false, errText};

   // Apply move.
   apply(*move);
   return {true, describeMove(*move)};
}

bool Game::canMove(Color side) const
{
   if (isMate(side))
      return false;

   Position copy = m_currPos;
   MoveCalculator calc{copy};
   const auto move = calc.next(side, 1);
   return move.has_value();
}

bool Game::isMate(Color side) const
{
   return matt2::isMate(side, m_currPos);
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

static std::pair<std::optional<Move>, std::string>
buildCastlingMoveFromCoords(const MoveDescription& descr, Color side)
{
   if (descr.from == Castling::from(side))
   {
      if (descr.to == Castling::to(Kingside, side))
         return {Castling{Kingside, side}, ""};
      if (descr.to == Castling::to(Queenside, side))
         return {Castling{Queenside, side}, ""};
   }

   // Not an error, more like a not chosen possibility.
   return {};
}

static std::pair<std::optional<Move>, std::string>
buildCastlingMove(const MoveDescription& descr, Color side)
{
   // If not special castling notation, check for castling coordinates.
   if (!descr.castling)
      return buildCastlingMoveFromCoords(descr, side);

   if (*descr.castling == MoveDescription::Castling::Kingside)
      return {Castling{Kingside, side}, ""};
   return {Castling{Queenside, side}, ""};
}

static std::optional<Piece> buildPromotedToPiece(MoveDescription::Promotion promoteTo,
                                                 Color side)
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
      return std::nullopt;
   }
}

static std::pair<std::optional<Move>, std::string>
buildPromotionMove(const MoveDescription& descr, const Position& pos, Color side)
{
   if (!descr.from || !descr.to || !descr.promoteTo)
      return {std::nullopt, "Invalid move locations."};

   const Square from = *descr.from;
   const Square to = *descr.to;

   const auto piece = pos[from];
   if (!piece)
      return {std::nullopt, "Invalid piece location."};

   const auto promoteTo = buildPromotedToPiece(*descr.promoteTo, side);
   if (!promoteTo)
      return {std::nullopt, "Invalid piece for promotion."};

   const auto taken = pos[to];

   // Validation of fields happens later.
   return {Promotion{*piece, from, to, *promoteTo, taken}, ""};
}

static std::pair<std::optional<Move>, std::string>
buildEnPassantMove(const MoveDescription& descr, const Position& pos, Color side)
{
   if (!descr.from || !descr.to)
      return {std::nullopt, "Invalid move locations."};

   const Square from = *descr.from;
   const Square to = *descr.to;

   const auto piece = pos[*descr.from];
   if (!piece)
      return {std::nullopt, "Invalid piece location."};
   else if (!isPawn(*piece))
      return {std::nullopt, "Invalid piece for en-passant move."};
   else if (color(*piece) != side)
      return {std::nullopt, "Wrong piece color."};

   // Checks to make sure it's an en-passant move. We don't want to misidentify a move.
   auto enPassantSq = pos.enPassantSquare();
   if (!enPassantSq)
      return {std::nullopt, "En-passant is not allowed for current position."};

   if (!isAdjacent(file(from), file(*enPassantSq)) && file(to) == file(*enPassantSq))
      return {std::nullopt, "Move file location are invalid for en-passant move."};

   if (rank(from) != rank(*enPassantSq) && rank(to) != nthRank(side, 6))
      return {std::nullopt, "Move rank location are invalid for en-passant move."};

   // Validation of fields happens later.
   return {EnPassant{*piece, from, to}, ""};
}

static std::pair<std::optional<Move>, std::string>
buildBasicMove(const MoveDescription& descr, const Position& pos)
{
   if (!descr.from || !descr.to)
      return {std::nullopt, "Invalid move locations."};

   const Square from = *descr.from;
   const Square to = *descr.to;

   const auto piece = pos[*descr.from];
   if (!piece)
      return {std::nullopt, "Invalid piece location."};

   const auto taken = pos[to];

   // Validation of fields happens later.
   return {BasicMove{*piece, from, to, taken}, ""};
}

std::pair<std::optional<Move>, std::string>
Game::buildMove(const MoveDescription& descr) const
{
   // Castling
   if (auto result = buildCastlingMove(descr, m_nextTurn); result.first.has_value())
      return result;

   // Promotion
   if (auto result = buildPromotionMove(descr, m_currPos, m_nextTurn);
       result.first.has_value())
      return result;

   // En-passant
   if (auto result = buildEnPassantMove(descr, m_currPos, m_nextTurn);
       result.first.has_value())
      return result;

   // Basic move
   return buildBasicMove(descr, m_currPos);
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
