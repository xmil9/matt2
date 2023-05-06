//
// Jul-2021, Michael Lindner
// MIT license
//
#include "rules.h"
#include <algorithm>
#include <cmath>
#include <functional>

using namespace matt2;


namespace
{
///////////////////

std::optional<Piece> collectBasicMove(Piece piece, Square from, Square to,
                                      const Position& pos, std::vector<Move>& moves)
{
   auto destPiece = pos[to];
   if (!destPiece || !haveSameColor(piece, *destPiece))
      moves.push_back(BasicMove{Relocation{piece, from, to}, destPiece});
   return destPiece;
}


template <std::size_t N>
void collectOffsetMoves(Piece piece, Square at, const Position& pos,
                        const std::array<Offset, N>& offsets, std::vector<Move>& moves)
{
   for (const auto& off : offsets)
      if (isOnBoard(at, off))
         collectBasicMove(piece, at, at + off, pos, moves);
}


template <std::size_t N>
void collectDirectionalMoves(Piece piece, Square at, const Position& pos,
                             const std::array<Offset, N>& directions,
                             std::vector<Move>& moves)
{
   for (const auto& off : directions)
   {
      Square to = at;
      std::optional<Piece> destPiece;

      // Keep moving into direction until the board ends or another piece is in
      // the way.
      while (isOnBoard(to, off) && !destPiece)
      {
         to = to + off;
         destPiece = collectBasicMove(piece, at, to, pos, moves);
      }
   }
}


bool isPromotion(Piece pawn, Square to)
{
   assert(isPawn(pawn));
   return rank(to) == (isWhite(pawn) ? r8 : r1);
}


void collectPromotions(Piece pawn, Square at, Square to, std::optional<Piece> taken,
                       std::vector<Move>& moves)
{
   // Add move for each possible promotion.
   // Note that validity of the moves has already been verified.
   using Promotions_t = std::array<Piece, 4>;
   static constexpr Promotions_t WhitePromotions = {Qw, Rw, Bw, Nw};
   static constexpr Promotions_t BlackPromotions = {Qb, Rb, Bb, Nb};

   const Relocation movement{pawn, at, to};

   const Promotions_t& promotions = isWhite(pawn) ? WhitePromotions : BlackPromotions;
   for (const Piece& promotedTo : promotions)
      moves.push_back(Promotion{movement, promotedTo, taken});
}


bool collectForwardPawnMove(Piece pawn, Square at, const Position& pos, Offset forward,
                            std::vector<Move>& moves)
{
   assert(isPawn(pawn));
   // To avoid duplicate checking, caller should check that destination square is on
   // the board.
   assert(isOnBoard(at, forward));

   bool moveCollected = false;

   Square to = at + forward;
   auto destPiece = pos[to];
   if (!destPiece)
   {
      assert(std::abs(forward.dr) == 1 || std::abs(forward.dr) == 2);
      if (std::abs(forward.dr) == 1)
      {
         if (isPromotion(pawn, to))
            collectPromotions(pawn, at, to, destPiece, moves);
         else
            moves.push_back(BasicMove{Relocation{pawn, at, to}});
      }
      else
      {
         // Moving by two squares allows opponent to use en-passant rule in the
         // next move.
         moves.push_back(BasicMove{Relocation{pawn, at, to}, EnablesEnPassant});
      }

      moveCollected = true;
   }

   return moveCollected;
}


void collectDiagonalPawnMove(Piece pawn, Square at, const Position& pos, Offset diagonal,
                             std::vector<Move>& moves)
{
   assert(isPawn(pawn));
   if (isOnBoard(at, diagonal))
   {
      Square to = at + diagonal;
      auto destPiece = pos[to];
      if (destPiece.has_value() && !haveSameColor(pawn, *destPiece))
      {
         if (isPromotion(pawn, to))
            collectPromotions(pawn, at, to, destPiece, moves);
         else
            moves.push_back(BasicMove{Relocation{pawn, at, to}, destPiece});
      }
   }
}


///////////////////

enum class SquareFilterPolicy
{
   OnlyEmpty,
   EmptyOrOpponent
};

using SquareFilterFn = std::function<bool(Square)>;

SquareFilterFn makeEmptySquareFilter(const Position& pos)
{
   return [&pos](Square at) { return !pos[at]; };
}

SquareFilterFn makeEmptyOrOpponentSquareFilter(Color opponent, const Position& pos)
{
   return [opponent, &pos](Square at)
   {
      const auto destPiece = pos[at];
      return !destPiece || color(*destPiece) == opponent;
   };
}

SquareFilterFn makeSquareFilter(SquareFilterPolicy policy, Piece piece,
                                const Position& pos)
{
   switch (policy)
   {
   case SquareFilterPolicy::OnlyEmpty:
      return makeEmptySquareFilter(pos);
   case SquareFilterPolicy::EmptyOrOpponent:
      return makeEmptyOrOpponentSquareFilter(!color(piece), pos);
   default:
      assert(false && "Unknown square filter policy.");
      return {};
   }
}


///////////////////

template <std::size_t N>
void collectOffsetSquares(Piece piece, Square at, const Position& pos,
                          SquareFilterPolicy policy, const std::array<Offset, N>& offsets,
                          std::vector<Square>& squares)
{
   SquareFilterFn filterFn = makeSquareFilter(policy, piece, pos);

   for (const auto& off : offsets)
      if (isOnBoard(at, off))
      {
         const Square to = at + off;
         if (filterFn(to))
            squares.push_back(to);
      }
}


template <std::size_t N>
void collectDirectionalSquares(Piece piece, Square at, const Position& pos,
                               SquareFilterPolicy policy,
                               const std::array<Offset, N>& directions,
                               std::vector<Square>& squares)
{
   SquareFilterFn filterFn = makeSquareFilter(policy, piece, pos);

   for (const auto& off : directions)
   {
      Square to = at;

      // Keep moving into direction until the board ends or another piece is in
      // the way.
      while (isOnBoard(to, off))
      {
         to = to + off;
         if (filterFn(to))
            squares.push_back(to);
         if (pos[to])
            break;
      }
   }
}


///////////////////

bool isPawnOnInitialRank(Piece pawn, Square at)
{
   const Rank initialRank = isWhite(pawn) ? r2 : r7;
   return rank(at) == initialRank;
}


bool areCastlingSquaresOccupied(Color side, bool onKingside, const Position& pos)
{
   // Squares between king and rook that cannot be occupied for each castling type.
   // Note that these are not the same as the squares that cannot be attacked for
   // castling.
   static const std::vector<Square> WhiteKingside = {f1, g1};
   static const std::vector<Square> WhiteQueenside = {b1, c1, d1};
   static const std::vector<Square> BlackKingside = {f8, g8};
   static const std::vector<Square> BlackQueenside = {b8, c8, d8};

   const std::vector<Square>& squares =
      side == White ? (onKingside ? WhiteKingside : WhiteQueenside)
                    : (onKingside ? BlackKingside : BlackQueenside);

   for (Square sq : squares)
      if (pos[sq].has_value())
         return true;
   return false;
}


bool areCastlingSquaresAttacked(Color side, bool onKingside, const Position& pos)
{
   // Squares that castling king moves across for each castling type, including the king's
   // initial square.
   // Note that castling can still happen if the rook is attacked or moving across an
   // attacked square (i.e. g1/g8 when castling queen-side).
   // Note that these are not the same as the squares that cannot be
   // occupied for castling.
   static const std::vector<Square> WhiteKingside = {e1, f1, g1};
   static const std::vector<Square> WhiteQueenside = {c1, d1, e1};
   static const std::vector<Square> BlackKingside = {e8, f8, g8};
   static const std::vector<Square> BlackQueenside = {c8, d8, e8};

   const std::vector<Square>& squares =
      side == White ? (onKingside ? WhiteKingside : WhiteQueenside)
                    : (onKingside ? BlackKingside : BlackQueenside);
   const Color opponent = !side;

   for (auto sq : squares)
      if (pos.canAttack(sq, opponent))
         return true;
   return false;
}


bool haveCastlingRook(Color side, bool onKingside, const Position& pos)
{
   const Square rookSq = makeSquare(onKingside ? fh : fa, side == White ? r1 : r8);
   const auto piece = pos[rookSq];
   return piece.has_value() && isRook(*piece) && color(*piece) == side;
}

} // namespace


namespace matt2
{
///////////////////

void collectKingMoves(Piece king, Square at, const Position& pos,
                      std::vector<Move>& moves)
{
   assert(isKing(king));

   std::vector<Move> possibleMoves;
   static constexpr std::array<Offset, 8> Offsets{
      Offset{1, 1}, {1, 0}, {1, -1}, {0, 1}, {0, -1}, {-1, 1}, {-1, 0}, {-1, -1}};
   collectOffsetMoves(king, at, pos, Offsets, possibleMoves);

   // Eliminate moves that would lead to check.
   std::vector<Square> attackedSorted;
   collectAttackedBySide(!color(king), pos, attackedSorted);

   std::copy_if(
      std::begin(possibleMoves), std::end(possibleMoves), std::back_inserter(moves),
      [&attackedSorted](const Move& m)
      {
         Square toSq = to(m);
         auto endIter = std::end(attackedSorted);
         auto geIter = std::lower_bound(std::begin(attackedSorted), endIter, toSq);
         // We still have to check for equality because the iterator returned by
         // lower_bound points to the first element that is not less than the search for
         // value.
         const bool isSquareAttacked = geIter != endIter && *geIter == toSq;
         return !isSquareAttacked;
      });
}


void collectQueenMoves(Piece queen, Square at, const Position& pos,
                       std::vector<Move>& moves)
{
   assert(isQueen(queen));
   static constexpr std::array<Offset, 8> Directions{
      Offset{1, 1}, {1, 0}, {1, -1}, {0, 1}, {0, -1}, {-1, 1}, {-1, 0}, {-1, -1}};
   collectDirectionalMoves(queen, at, pos, Directions, moves);
}


void collectRookMoves(Piece rook, Square at, const Position& pos,
                      std::vector<Move>& moves)
{
   assert(isRook(rook));
   static constexpr std::array<Offset, 4> Directions{
      Offset{1, 0}, {0, 1}, {0, -1}, {-1, 0}};
   collectDirectionalMoves(rook, at, pos, Directions, moves);
}


void collectBishopMoves(Piece bishop, Square at, const Position& pos,
                        std::vector<Move>& moves)
{
   assert(isBishop(bishop));
   static constexpr std::array<Offset, 4> Directions{
      Offset{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
   collectDirectionalMoves(bishop, at, pos, Directions, moves);
}


void collectKnightMoves(Piece knight, Square at, const Position& pos,
                        std::vector<Move>& moves)
{
   assert(isKnight(knight));
   static constexpr std::array<Offset, 8> Offsets{
      Offset{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
   collectOffsetMoves(knight, at, pos, Offsets, moves);
}


void collectPawnMoves(Piece pawn, Square at, const Position& pos,
                      std::vector<Move>& moves)
{
   assert(isPawn(pawn));

   // Move one square forward if square is not occupied.
   bool couldMoveForward = false;
   const Offset forward{0, isWhite(pawn) ? 1 : -1};
   if (isOnBoard(at, forward))
      couldMoveForward = collectForwardPawnMove(pawn, at, pos, forward, moves);

   // Move two squares forward if pawn is on initial rank and no other piece is
   // in front of it.
   // Note that checking for the initial rank also covers checking whether the
   // destination square is on the board.
   if (isPawnOnInitialRank(pawn, at) && couldMoveForward)
   {
      const Offset forwardBy2{0, isWhite(pawn) ? 2 : -2};
      collectForwardPawnMove(pawn, at, pos, forwardBy2, moves);
   }

   // Add moves for taking pieces diagonally.
   const Offset diagonalRight{1, forward.dr};
   collectDiagonalPawnMove(pawn, at, pos, diagonalRight, moves);
   const Offset diagonalLeft{-1, forward.dr};
   collectDiagonalPawnMove(pawn, at, pos, diagonalLeft, moves);
}

void collectMoves(Piece piece, Square at, const Position& pos, std::vector<Move>& moves)
{
   if (isKing(piece))
      collectKingMoves(piece, at, pos, moves);
   else if (isQueen(piece))
      collectQueenMoves(piece, at, pos, moves);
   else if (isRook(piece))
      collectRookMoves(piece, at, pos, moves);
   else if (isBishop(piece))
      collectBishopMoves(piece, at, pos, moves);
   else if (isKnight(piece))
      collectKnightMoves(piece, at, pos, moves);
   else if (isPawn(piece))
      collectPawnMoves(piece, at, pos, moves);
   else
      throw std::runtime_error("Unknown piece.");
}

void collectCastlingMoves(Color side, const Position& pos, std::vector<Move>& moves)
{
   if (canCastle(side, true, pos))
      moves.push_back(Castling{Kingside, side});
   if (canCastle(side, false, pos))
      moves.push_back(Castling{Queenside, side});
}


void collectEnPassantMoves(Color side, const Position& pos, std::vector<Move>& moves)
{
   // Is en-passant enabled?
   const auto epSquare = pos.enPassantSquare();
   if (!epSquare.has_value())
      return;
   const File epFile = file(*epSquare);

   // Is en-passant enabled by opponent?
   const auto epPiece = pos[*epSquare];
   if (!epPiece.has_value() || color(*epPiece) == side)
      return;

   const auto fromRank = side == White ? r5 : r4;
   const auto toRank = side == White ? r6 : r3;
   Square to = makeSquare(epFile, toRank);

   // Try both neighboring files.
   static constexpr std::array<int, 2> FileOffsets = {-1, 1};
   for (int off : FileOffsets)
   {
      // Is the neighboring file on the board?
      if (isValid(epFile, off))
      {
         const Square from = makeSquare(epFile + off, fromRank);

         // Is a pawn of matching color on the right square to make an en-passent
         // move?
         if (const auto piece = pos[from];
             piece.has_value() && isPawn(*piece) && color(*piece) == side)
         {
            moves.push_back(EnPassant{Relocation(*piece, from, to)});
         }
      }
   }
}

///////////////////

void collectAttackedByKing(Piece king, Square at, const Position& pos,
                           std::vector<Square>& attacked)
{
   assert(isKing(king));
   static constexpr std::array<Offset, 8> Offsets{
      Offset{1, 1}, {1, 0}, {1, -1}, {0, 1}, {0, -1}, {-1, 1}, {-1, 0}, {-1, -1}};
   collectOffsetSquares(king, at, pos, SquareFilterPolicy::EmptyOrOpponent, Offsets,
                        attacked);
}


void collectAttackedByQueen(Piece queen, Square at, const Position& pos,
                            std::vector<Square>& attacked)
{
   assert(isQueen(queen));
   static constexpr std::array<Offset, 8> Directions{
      Offset{1, 1}, {1, 0}, {1, -1}, {0, 1}, {0, -1}, {-1, 1}, {-1, 0}, {-1, -1}};
   collectDirectionalSquares(queen, at, pos, SquareFilterPolicy::EmptyOrOpponent,
                             Directions, attacked);
}


void collectAttackedByRook(Piece rook, Square at, const Position& pos,
                           std::vector<Square>& attacked)
{
   assert(isRook(rook));
   static constexpr std::array<Offset, 4> Directions{
      Offset{1, 0}, {0, 1}, {0, -1}, {-1, 0}};
   collectDirectionalSquares(rook, at, pos, SquareFilterPolicy::EmptyOrOpponent,
                             Directions, attacked);
}

void collectAttackedByBishop(Piece bishop, Square at, const Position& pos,
                             std::vector<Square>& attacked)
{
   assert(isBishop(bishop));
   static constexpr std::array<Offset, 4> Directions{
      Offset{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
   collectDirectionalSquares(bishop, at, pos, SquareFilterPolicy::EmptyOrOpponent,
                             Directions, attacked);
}

void collectAttackedByKnight(Piece knight, Square at, const Position& pos,
                             std::vector<Square>& attacked)
{
   assert(isKnight(knight));
   static constexpr std::array<Offset, 8> Offsets{
      Offset{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
   collectOffsetSquares(knight, at, pos, SquareFilterPolicy::EmptyOrOpponent, Offsets,
                        attacked);
}

void collectAttackedByEnPassant(Piece pawn, Square at, const Position& pos,
                                std::vector<Square>& attacked)
{
   assert(isPawn(pawn));

   // Is en-passant enabled?
   const auto epSquare = pos.enPassantSquare();
   if (!epSquare.has_value())
      return;

   // Is en-passant enabled by opponent?
   const auto epPiece = pos[*epSquare];
   if (!epPiece.has_value() || haveSameColor(pawn, *epPiece))
      return;

   // Is the attacking pawn on a square that it can do the en-passant move from?
   if (rank(at) == rank(*epSquare) && isAdjacent(file(at), file(*epSquare)))
      attacked.push_back(*epSquare);
}

void collectAttackedByPawn(Piece pawn, Square at, const Position& pos,
                           std::vector<Square>& attacked)
{
   assert(isPawn(pawn));
   const int forward = isWhite(pawn) ? 1 : -1;
   const std::array<Offset, 2> Offsets{Offset{1, forward}, {-1, forward}};
   collectOffsetSquares(pawn, at, pos, SquareFilterPolicy::EmptyOrOpponent, Offsets,
                        attacked);

   collectAttackedByEnPassant(pawn, at, pos, attacked);
}

void collectAttackedBySide(Color side, const Position& pos, std::vector<Square>& attacked)
{
   auto endIter = pos.end(side);
   for (auto iter = pos.begin(side); iter < endIter; ++iter)
   {
      const Piece piece = iter.piece();
      if (isKing(piece))
         collectAttackedByKing(piece, iter.at(), pos, attacked);
      else if (isQueen(piece))
         collectAttackedByQueen(piece, iter.at(), pos, attacked);
      else if (isRook(piece))
         collectAttackedByRook(piece, iter.at(), pos, attacked);
      else if (isBishop(piece))
         collectAttackedByBishop(piece, iter.at(), pos, attacked);
      else if (isKnight(piece))
         collectAttackedByKnight(piece, iter.at(), pos, attacked);
      else if (isPawn(piece))
         collectAttackedByPawn(piece, iter.at(), pos, attacked);
      else
         throw std::runtime_error("Unknown piece.");
   }

   // Eliminate duplicates.
   std::sort(attacked.begin(), attacked.end());
   attacked.erase(std::unique(attacked.begin(), attacked.end()), attacked.end());
}

///////////////////

bool canCastle(Color side, bool onKingside, const Position& pos)
{
   return !pos.hasKingMoved(side) && haveCastlingRook(side, onKingside, pos) &&
          !pos.hasRookMoved(side, onKingside) &&
          !areCastlingSquaresOccupied(side, onKingside, pos) &&
          !areCastlingSquaresAttacked(side, onKingside, pos);
}

bool isCheck(Color side, const Position& pos)
{
   const auto kingSq = pos.kingLocation(side);
   if (!kingSq)
      return true;

   std::vector<Square> attacked;
   collectAttackedBySide(!side, pos, attacked);

   return std::find(attacked.begin(), attacked.end(), *kingSq) != attacked.end();
}

bool isMate(Color side, const Position& pos)
{
   return pos.count(king(side)) == 0;
}

} // namespace matt2
